#!/usr/bin/env ruby
# frozen_string_literal: true

# Copyright 2023-2023, Exprivia SpA.
# Via Della Bufalotta, 378 - 00139 Roma - Italy
# http://www.exprivia.com

# All Rights Reserved.

# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
# the contents of this file may not be disclosed to third parties, copied or
# duplicated in any form, in whole or in part, without the prior written
# permission of Exprivia SpA.

# $Prod: ADGSCatalogueExport Tests $

# :nocov:
# Add current dir to load path if not already included
$LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'common_tests_utils'

require 'configuration_handler_tests'
require 'db_handler_tests'
require 'report_generator_tests'
require 'report_handler_tests'
