# frozen_string_literal: true

# Copyright 2021-2023, Exprivia SpA.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com
#
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Exprivia SpA.
#
# $Prod: RubyLibs $

require 'syslog'

# Handle the SPLUNK message
module SPLUNKHandler
  # Send a SPLUNK message
  def self.emit_syslog(ident, smt_msg)
    raise TypeError, "Argument 'ident' must be a String" unless ident.is_a?(String)
    raise TypeError, "Argument 'smt_msg' must be a String" unless smt_msg.is_a?(String)

    # sys_log = Syslog.open(ident)
    sys_log = Syslog.open(ident, Syslog::LOG_CONS | Syslog::LOG_PID, Syslog::LOG_SYSLOG)
    sys_log.notice(smt_msg)
  ensure
    sys_log.close unless sys_log.nil?
  end
end
