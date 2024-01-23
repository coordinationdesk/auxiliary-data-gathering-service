#!/usr/bin/python

import argparse
import json
import logging
from logging import handlers
import os
import requests
from requests.auth import HTTPBasicAuth
import shutil
import sys
import textwrap
import time

import yaml

from configparser import ConfigParser, NoOptionError, NoSectionError
from enum import Enum


class Config(Enum):
    BASE_URL = 'baseUrl'
    API_VERSION = 'apiVer'
    USER_NAME = 'username'
    TOKEN = 'token'
    ASSIGNEE = 'assignee'
    STATUS = 'status'
    TYPE = 'type'
    TRANSITION_ID = 'transId'
    OUTPUT_DIR = 'OutDirectory'
    LOG_DIR = 'LogDirectory'
    LOG_LEVEL = 'LogLevel'


PROJECT = 'ADGS'
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

HELP = {
    'conf': 'The YAML configuration file (default: %(default)s)',
}

DEFAULTS = {
    'conf': os.path.join(BASE_DIR, 'conf.yaml'),
}


class ADGS_JiraIssueDownloaderException(Exception): pass

class ConfigurationError(ADGS_JiraIssueDownloaderException): pass

class GetIssuesError(ADGS_JiraIssueDownloaderException): pass

class GetAttachmentsError(ADGS_JiraIssueDownloaderException): pass

class DownloadError(ADGS_JiraIssueDownloaderException): pass

class DownloadFileError(DownloadError): pass

class MoveFileError(DownloadError): pass

class CloseIssueError(ADGS_JiraIssueDownloaderException): pass


class App:
    """
    A class that accepts a configuration file from the command line in yaml format.
    """
    description = 'Issue Attachment Downloader App'

    def __init__(self, *args, **kwargs):
        self.parser = argparse.ArgumentParser(
            description=self.description,
            formatter_class=argparse.RawTextHelpFormatter
        )
        self.parser.add_argument('-c', '--conf', default=DEFAULTS['conf'], help=textwrap.dedent(HELP['conf']))

        try:
            self.conf = self._parse_conf()
        except ConfigurationError as exc:
            sys.stderr.write(str(exc))
            sys.exit(1)

    def _parse_conf(self):
        args = self.parser.parse_args()
        conf_file = args.conf

        if not os.path.exists(conf_file):
            raise ConfigurationError('File {} does not exist.\n'.format(conf_file))

        with open(conf_file, 'r') as stream:
            try:
                return yaml.safe_load(stream) or {}
            except yaml.YAMLError as exc:
                raise ConfigurationError('Error parsing configuration file.\n')

    def run(self, *args, **kwargs):
        raise NotImplementedError


def init_logging(name, path='~/logs', level=logging.INFO):
    filename = os.path.expanduser(f'{os.path.join(path, name)}.log')
    handler = handlers.TimedRotatingFileHandler(
        filename=filename,
        when='midnight',
        backupCount=14
    )
    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(filename)s[%(lineno)s] - %(message)s')
    handler.setFormatter(formatter)
    root = logging.getLogger()
    root.setLevel(level)
    root.addHandler(handler)

    # Mute 'requests' logger a bit
    logging.getLogger("requests").setLevel(logging.WARNING)


class ADGS_JiraIssueDownloader(App):

    def get_issues(self, auth):
        pconf = self.conf.get(PROJECT, [])
        baseUrl = pconf[Config.BASE_URL.value]
        apiVer = pconf[Config.API_VERSION.value]
        status = pconf[Config.STATUS.value]
        assignee = pconf[Config.ASSIGNEE.value]
        itype = pconf[Config.TYPE.value]
        logging.info(f"Searching for all issues of user {assignee} in status '{status}' of type '{itype}'")

        url = f"{baseUrl}/rest/api/{apiVer}/search?jql=project={PROJECT}+and+status=\"{status}\"+and+assignee=\"{assignee}\"+and+type=\"{itype}\""
        headers = {
            "Accept": "application/json"
        }

        logging.debug(f'Calling url {url}')
        response = requests.get(url, headers=headers, auth=auth)
        logging.debug(f"Status Code {response.status_code}")
        try: 
            response.raise_for_status()
        except requests.exceptions.HTTPError as exc: 
            msg = f"Error getting issues list: {str(exc)}"
            logging.error(msg)
            raise GetIssuesError(msg)

        # return issues of required user
        return json.loads(response.text)

    def get_attachments(self, auth, issue):
        logging.info(f"Searching for all attachments of issue {issue}")

        pconf = self.conf.get(PROJECT, [])
        baseUrl = pconf[Config.BASE_URL.value]
        apiVer = pconf[Config.API_VERSION.value]

        url = f"{baseUrl}/rest/api/{apiVer}/issue/{issue}"
        headers = {
            "Accept": "application/json"
        }

        logging.debug(f'Calling url {url}')
        response = requests.get(url, headers=headers, auth=auth)
        logging.debug(f"Status Code {response.status_code}")
        try:
            response.raise_for_status()
        except requests.exceptions.HTTPError as exc: 
            msg = f"Error getting attachments list for issue {issue}: {str(exc)}"
            logging.error(msg)
            raise GetAttachmentsError(msg)

        # return required issue attachments
        issueInfo = json.loads(response.text)
        noresults = not 'attachment' in issueInfo['fields'] or len(issueInfo['fields']['attachment']) == 0
        return [] if noresults else issueInfo['fields']['attachment']

    def download(self, auth, url, filename):
        logging.info(f"Downloading attachment {filename}")

        # download the attachment
        logging.debug(f'Calling url {url}')
        response = requests.get(url, auth=auth, stream=True)
        logging.debug(f"Status Code {response.status_code}")
        try: 
            response.raise_for_status()
        except requests.exceptions.HTTPError as exc: 
            msg = f"Error downloading file {filename}: {str(exc)}"
            logging.error(msg)
            raise DownloadFileError(msg)

        outpath = self.conf[PROJECT][Config.OUTPUT_DIR.value]
        now = str(int(round(time.time() * 1000)))
        tmp_file = f'{outpath}/{now}_{filename}'

        with open(tmp_file, "wb") as f: 
            f.write(response.content.decode('iso-8859-1').encode('utf8'))
        f.close()

        # move the attachment file to the output directory
        try:
            shutil.move(tmp_file, f"{outpath}/{filename}")
        except:
            os.remove(tmp_file)
            msg = f"Error moving file {filename} to output directory {outpath}"
            logging.error(msg)
            raise MoveFileError(msg)

    def close_issue(self, auth, issue):
        logging.info(f"Closing issue {issue}")

        pconf = self.conf.get(PROJECT, [])
        baseUrl = pconf[Config.BASE_URL.value]
        apiVer = pconf[Config.API_VERSION.value]
        transId = pconf[Config.TRANSITION_ID.value]
        url = f"{baseUrl}/rest/api/{apiVer}/issue/{issue}/transitions"

        headers = {
            "Accept": "application/json",
            "Content-Type": "application/json"
        }

        payload = json.dumps( {
            "transition": { "id": str(transId) }
        } )

        logging.debug(f'Calling url {url}')
        response = requests.post(url, data=payload, headers=headers, auth=auth)
        logging.debug(f"Status Code {response.status_code}")
        try:
            response.raise_for_status()
        except requests.exceptions.HTTPError as exc: 
            msg = f"Error closing issue {issue}: {str(exc)}"
            logging.error(msg)
            raise CloseIssueError(msg)

    def run(self, *args, **kwargs):
        pconf = self.conf.get(PROJECT, [])
        username = pconf[Config.USER_NAME.value]
        token = pconf[Config.TOKEN.value]
        logpath = pconf[Config.LOG_DIR.value]
        loglevel = logging.INFO if not Config.LOG_LEVEL.value in pconf else pconf[Config.LOG_LEVEL.value]
        auth = HTTPBasicAuth(username, token)

        init_logging('ADGS_JiraIssueDownloader', logpath, loglevel)
        logging.info('Starting')

        # get all project issues for specified user
        issues = self.get_issues(auth)
        found = len(issues['issues'])
        logging.info(f'{found} issue/s found')

        if found == 0:
            logging.info('Ending')
            return

        iclosed = 0
        for issue in issues['issues']:
            issuename = issue['key']
            logging.info(f"Processing issue {issuename}...")

            # get current issue attachments
            attachments = self.get_attachments(auth, issuename)

            if len(attachments) == 0:
                logging.info(f"Issue {issuename} has no attachments")
                # if no attachments, no error occurs, current issue will not be closed
                continue

            # download issue attachments into the output path
            nextissue = False
            for attachment in attachments:
                try:
                    self.download(auth, attachment['content'], attachment['filename'])
                except DownloadError as exc:
                    logging.error(str(exc))
                    nextissue = True
                    break
            if nextissue:
                nextissue = False
                continue

            # close the issue
            try:
                self.close_issue(auth, issuename)
            except CloseIssueError as exc:
                continue

            iclosed += 1

        logging.info(f'Closed {iclosed}of{found} found issue/s')
        logging.info('Ending')


def main(argv):

    # configuration and log inizialization inside app
    app = ADGS_JiraIssueDownloader()
    app.run(argv)


if __name__ == "__main__":
    main(sys.argv[1:])
