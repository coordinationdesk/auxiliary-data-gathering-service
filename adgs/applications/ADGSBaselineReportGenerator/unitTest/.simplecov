# rubocop:disable Naming/FileName
# rubocop:enable Naming/FileName
# frozen_string_literal: true

# Copyright 2023-2023, Exprivia SpA.
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
# $Prod: ADGSBaselineReportGenerator Tests $

require 'rubygems'

have_branch_coverage = Gem::Version.new(RUBY_VERSION) >= Gem::Version.new('2.5.0')

SimpleCov.root(File.join(__dir__, '..'))
SimpleCov.start do
  enable_coverage :branch if have_branch_coverage
  add_filter '/test/'
  add_filter '/unitTest/'
end
