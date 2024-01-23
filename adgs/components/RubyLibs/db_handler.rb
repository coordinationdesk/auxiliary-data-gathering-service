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

# :nocov:
# Add the lib main to load path if not already included
$LOAD_PATH.unshift(__dir__) unless $LOAD_PATH.include?(__dir__)
# :nocov:

require 'open3'
require 'pg'
require 'singleton'

require 'common_utils'

# Handle the configuration and the connection with a DB
module DBHandler
  # Load the database configuration
  class DBConfiguration
    ######################
    # INSTANCE ATTRIBUTE #
    ######################

    attr_reader :hostname, :schema, :user, :password

    ####################
    # INSTANCE METHODS #
    ####################

    # Load the given db.conf file
    def initialize(db_conf_file, host_index: 0, decrypter: nil)
      db_conf = File.read(db_conf_file)
      @hostname = get_param(db_conf, host_index, 'hostname')
      @schema = get_param(db_conf, host_index, 'db')
      @user = get_param(db_conf, host_index, 'user')
      @password = get_param(db_conf, host_index, 'password')

      check_password_encrypted(db_conf, host_index, decrypter)
    end

    ############################
    # PRIVATE INSTANCE METHODS #
    ############################

    private

    # Set the password and decrypt it if needed
    def check_password_encrypted(db_conf, host_index, decrypter)
      pass_enc = get_param(db_conf, host_index, 'passwordEncryptionAlgo', mandatory: false)

      # TODO: Handle all the cases

      # All done if the password is not encrypted
      return if pass_enc.nil? || pass_enc.to_i == 0
      raise 'Encrypted password on config file. Decrypter path needed.' unless decrypter.is_a?(String)

      @password, err, status = Open3.capture3("#{decrypter} -b #{@password}")
      @password.strip!
      raise "Decryption failed with error: #{err}" unless err.empty?
      raise 'Error while decrypting password.' unless status.success?
    end

    # Get a param from the db_conf file
    def get_param(db_conf, host_index, param, mandatory: true)
      data = db_conf.match(/^Postgres\.host\[#{host_index}\]\.dbConnectionArbiterCParam\.#{param}:\s*(?<value>\S*)\s*$/)

      if data.nil?
        raise(LoadError, "Unable to get '#{param}' from DB config file.") if mandatory

        return nil
      end
      data[:value]
    end
  end

  # A generic fields (or tables) list
  class FieldsList
    # Initialize the list
    def initialize(field: nil)
      @list = []

      add(field) unless field.nil?
    end

    # Set the fields values
    def list=(field)
      if field.is_a?(String)
        @list = [field]
      elsif field.is_a?(Array) && field.all? { |f| f.is_a?(String) }
        @list = field
      else
        raise TypeError, "Argument 'field' must be a String or an Array of Strings"
      end
    end

    # Add a new field
    def add(field)
      check_type(field, 'field', [String])

      @list.push(field)
      self
    end

    # Check if the fields list is empty
    def empty?
      @list.empty?
    end

    # Get the size of the fields list
    def size
      @list.size
    end

    # Compose the fields list by concatenate them in a comma separated string
    def compose
      @list.join(', ')
    end
  end

  # A generic values list
  class ValuesList
    # Initialize the list
    def initialize(value: [])
      @list = []

      add(value) unless value.is_a?(Array)
    end

    # Set the values
    def list=(value)
      @list = if value.is_a?(Array)
                value.each_with_index do |el, n|
                  check_type(el, "value[#{n}]", [String, Numeric], nil_allowed: true, boolean_allowed: true)
                end
                value
              else
                check_type(value, 'value', [String, Numeric], nil_allowed: true, boolean_allowed: true)

                [value]
              end
    end

    # Add a new value
    def add(value)
      check_type(value, 'value', [String, Numeric], nil_allowed: true, boolean_allowed: true)
      @list.push(value)
      self
    end

    # Check if the values list is empty
    def empty?
      @list.empty?
    end

    # Get the size of the values list
    def size
      @list.size
    end

    # Compose the values list by convert and concatenate them in a comma separated string
    def compose
      @list.map { |el| convert_values(el) }.join(', ')
    end

    # Convert a single element of the list
    def convert_values(element)
      case element
      when String
        "'#{element}'"
      when nil
        'NULL'
      else
        element.to_s
      end
    end
  end

  # A JOIN clause in a query
  class JoinClause
    TYPES = %i[inner left right full cross].freeze

    # Initialize the JOIN clause
    def initialize
      @join_elements = []
    end

    # Add a new JOIN clause to the list
    def add_join(table, type: :inner, outer: false, on: nil)
      check_type(table, 'table', [String])
      raise TypeError, "Argument 'type' must be one of #{TYPES}" unless TYPES.include?(type)
      raise TypeError, "Argument 'outer' must be a Boolean" unless [true, false].include?(outer)
      raise TypeError, "Argument 'on' must be a WhereClause" unless type == :cross ? on.nil? : on.is_a?(WhereClause)

      @join_elements.push({ table: table, type: type, outer: outer, on: on })
      self
    end

    # Compose the JOIN clause
    def compose
      raise IndexError, 'JoinClause is empty' if @join_elements.empty?

      join_clause = ''
      @join_elements.each { |clause| join_clause += compose_element(clause) }
      join_clause
    end

    private

    def compose_element(element)
      result = case element[:type]
               when :inner then ' INNER JOIN '
               when :left then " LEFT#{compose_outer(element[:outer])} JOIN "
               when :right then " RIGHT#{compose_outer(element[:outer])} JOIN "
               when :full then " FULL#{compose_outer(element[:outer])} JOIN "
               when :cross then ' CROSS JOIN '
               # :nocov:
               else raise 'Unknown JOIN type.'
                 # :nocov:
               end
      "#{result}#{element[:table]}#{compose_on(element[:on]) unless element[:type] == :cross}"
    end

    # Add OUTER clause if needed
    def compose_outer(outer)
      outer ? ' OUTER' : ''
    end

    # Add ON clause
    def compose_on(on_clause)
      " ON #{on_clause.compose}"
    end
  end

  # A WHERE clause in a query
  class WhereClause
    OPERATORS = { eq: '=', ne: '!=', lt: '<', le: '<=', gt: '>', ge: '>=', in: 'IN', not_in: 'NOT IN', null: 'IS NULL',
                  not_null: 'IS NOT NULL', like: 'LIKE' }.freeze

    ####################
    # INSTANCE METHODS #
    ####################

    # Initialization
    def initialize(is_and: true)
      raise TypeError, "Argument 'is_and' must be a Boolean" unless [true, false].include?(is_and)

      @is_and = is_and
      @where_elements = []
    end

    # Set the combination operator of inner elements to OR
    def set_or
      @is_and = false
      self
    end

    # Set the combination operator of inner elements to AND
    def set_and
      @is_and = true
      self
    end

    # Add a single WHERE element
    def add_element(field, operator, value = nil)
      check_type(field, 'field', [String])
      raise TypeError, "Argument 'operator' must be one of #{OPERATORS}" unless OPERATORS.include?(operator)

      @where_elements.push({ field: field, operator: operator, value: check_value(operator, value) })
      self
    end

    # Add a nested WHERE clause
    def add_nested_clauses(clause)
      check_type(clause, 'clause', [WhereClause])

      @where_elements.push(clause)
      self
    end

    # Compose the WHERE clause
    def compose(nested: false)
      raise IndexError, 'WhereClause is empty' if @where_elements.empty?

      operator = @is_and ? ' AND ' : ' OR '
      ret_str = ''
      @where_elements.each do |element|
        ret_str += operator unless ret_str.empty?
        ret_str += compose_element(element)
      end
      ret_str = "( #{ret_str} )" if @where_elements.size > 1 || nested
      ret_str
    end

    ############################
    # PRIVATE INSTANCE METHODS #
    ############################

    private

    # Compose single WHERE element
    def compose_element(element)
      if element.is_a?(WhereClause)
        element.compose(nested: true)
      else
        result = "#{element[:field]} #{OPERATORS[element[:operator]]}"
        return result if %i[null not_null].include?(element[:operator])
        return "#{result} ( #{element[:value].compose(nested: true)} )" if element[:value].is_a?(Query)

        "#{result} #{element[:value]}"
      end
    end

    # Check value
    def check_value(operator, value)
      msg, classes = check_classes(operator)
      raise TypeError, "#{msg} for #{OPERATORS[operator]} operator" unless classes.any? { |c| value.is_a?(c) }

      value.is_a?(Array) ? compose_in_values(value) : value
    end

    # Select the allowed classes and the error message
    def check_classes(operator)
      case operator
      when :null, :not_null
        ["Argument 'value' must be nil", [NilClass]]
      when :in, :not_in
        ["Argument 'value' must be a Array or a Query", [Array, Query]]
      when :like
        ["Argument 'value' must be a String", [String]]
      else
        ["Argument 'value' must be a String, a Numeric or a Query", [String, Numeric, Query]]
      end
    end

    # Compose the IN operator values
    def compose_in_values(values)
      in_value = ''
      values.each do |v|
        in_value += ', ' unless in_value.empty?
        in_value += v.is_a?(String) ? "'#{v}'" : v.to_s
      end
      "(#{in_value})"
    end
  end

  # Parameters in the Query class
  module QueryParams
    TYPES = %i[select delete insert update].freeze

    attr_reader :fields, :from, :join, :where, :group_by, :order_by, :limit, :offset, :tables, :values, :returning

    # Set the fields to a single field or an array of fields
    def fields=(new_fields)
      @fields.list = new_fields
    end

    # Set the table or an array of tables for the FROM clause
    def from=(new_tables)
      @from.list = new_tables
    end

    # Set the JOIN clause
    def join=(new_join)
      check_type(new_join, 'new_join', [JoinClause], nil_allowed: true)

      @join = new_join
    end

    # Set the WHERE clause
    def where=(new_where)
      check_type(new_where, 'new_where', [WhereClause], nil_allowed: true)

      @where = new_where
    end

    # Set the GROUP BY clause
    def group_by=(new_group_by)
      @group_by.list = new_group_by
    end

    # Set the ORDER BY clause
    def order_by=(new_order_by)
      @order_by.list = new_order_by
    end

    # Set the LIMIT clause
    def limit=(new_limit)
      check_type(new_limit, 'new_limit', [Integer], nil_allowed: true)

      @limit = new_limit
    end

    # Set the OFFSET clause
    def offset=(new_offset)
      check_type(new_offset, 'new_offset', [Integer], nil_allowed: true)

      @offset = new_offset
    end

    # Set the table to a new value
    def table=(new_table)
      check_type(new_table, 'new_table', [String])
      @table = new_table
    end

    # Set the values to a single value or an array of values
    def values=(new_value)
      if new_value.is_a?(Query)
        @values = new_value
      else
        @values = ValuesList.new if @values.is_a?(Query)
        @values.list = new_value
      end
    end

    # Set the returning to a new string or nil
    def returning=(new_returning)
      check_type(new_returning, 'new_returning', [String])
      @returning = new_returning
    end
  end

  # A generic query
  class Query
    include QueryParams

    # Initialize the query by choosing the type
    def initialize(type: :select)
      raise TypeError, "Argument 'type' must be one of #{TYPES}" unless TYPES.include?(type)

      @type = type
      initialize_select_delete_fields
      initialize_insert_update_fields
    end

    # Compose the query
    def compose(nested: false)
      # TODO: set mandatory fields
      r = case @type
          when :select then compose_select
          when :delete then compose_delete
          when :insert then compose_insert
          when :update then compose_update
          # :nocov:
          else raise 'Unknown Query type.'
            # :nocov:
          end
      nested ? r : "#{r};"
    end

    private

    # initialize fields used for select and delete
    def initialize_select_delete_fields
      @fields = FieldsList.new
      @from = FieldsList.new
      @join = nil
      @where = nil
      @group_by = FieldsList.new
      @order_by = FieldsList.new
      @limit = nil
      @offset = nil
    end

    # initialize fields used for insert and update
    def initialize_insert_update_fields
      @table = ''
      @values = ValuesList.new
      @returning = nil
    end

    # Compose a SELECT query
    def compose_select
      "SELECT #{c_fields}#{c_from}#{c_join}#{c_where}#{c_group}#{c_order}#{c_limit}#{c_offset}"
    end

    # Compose a DELETE query
    def compose_delete
      "DELETE#{c_from}#{c_where}#{c_returning}"
    end

    # Compose a INSERT query
    def compose_insert
      "INSERT INTO #{@table} (#{c_fields}) VALUES (#{@values.compose})#{c_returning}"
    end

    # Compose a UPDATE query
    def compose_update
      "UPDATE #{@table} SET #{c_fields_values}#{c_from}#{c_where}#{c_returning}"
    end

    # Concatenate the fields
    def c_fields
      @fields.empty? ? '' : @fields.compose
    end

    # Compose the FROM clause
    def c_from
      @from.empty? ? '' : " FROM #{@from.compose}"
    end

    # Compose the JOIN clause
    def c_join
      @join.nil? ? '' : @join.compose
    end

    # Compose the WHERE clause
    def c_where
      @where.nil? ? '' : " WHERE #{@where.compose}"
    end

    # Compose the GROUP BY clause
    def c_group
      @group_by.empty? ? '' : " GROUP BY #{@group_by.compose}"
    end

    # Compose the ORDER BY clause
    def c_order
      @order_by.empty? ? '' : " ORDER BY #{@order_by.compose}"
    end

    # Compose the LIMIT clause
    def c_limit
      @limit.nil? ? '' : " LIMIT #{@limit}"
    end

    # Compose the OFFSET clause
    def c_offset
      @offset.nil? ? '' : " OFFSET #{@offset}"
    end

    # Compose the RETURNING clause
    def c_returning
      return '' if @returning.nil? || @returning.empty?

      " RETURNING #{@returning}"
    end

    # Compose the SET clause (fields = values) for UPDATE type
    def c_fields_values
      return "(#{c_fields}) = (#{@values.compose(nested: true)})" if @values.is_a?(Query)

      raise "Parameters 'fields' and 'values' must have the same size" if @fields.size != @values.size

      @fields.size == 1 ? "#{c_fields} = #{@values.compose}" : "(#{c_fields}) = ROW (#{@values.compose})"
    end
  end

  # Manage the connection with a DB
  class DB
    attr_reader :hostname, :dbname, :user, :password

    # Load the given db.conf file
    def initialize(db_conf_file, host_index: 0, decrypter: nil)
      conf = DBConfiguration.new(db_conf_file, host_index: host_index, decrypter: decrypter)
      @hostname = conf.hostname
      @dbname = conf.schema
      @user = conf.user
      @password = conf.password
    end

    # Perform a select query
    def exec(query, silent: false, no_throw: true, & block)
      check_type(query, 'query', [Query])

      # Execute the query
      rs = execute_query(query.compose, silent, no_throw)

      # Do operation on the query, if any
      rs.each(&block) if block_given? && !rs.nil?

      # Return the query result
      rs
    end

    private

    # Execute a given query
    def execute_query(query, silent, no_throw)
      con = PG.connect(host: @hostname, dbname: @dbname, user: @user, password: @password)

      con.exec(query)
    rescue StandardError => e
      print_error(e) unless silent
      raise unless no_throw

      nil
    ensure
      # :nocov:
      con.close unless con.nil?
      # :nocov:
    end
  end

  # Load and store the look-up tables requested
  class LookUpTables
    include Singleton

    # initialize the table map
    def initialize
      @tables_map = Hash.new { |hash, key| hash[key] = {} }
    end

    # Load a table, store his id/name pair and return a copy of it
    def get(db, table, id_column, name_column, silent: false)
      db_id = db_identifier(db)
      load(db, table, id_column, name_column, silent) unless @tables_map.key?(db_id) && @tables_map[db_id].key?(table)

      @tables_map[db_id][table]
    end

    private

    # Unique identifier for the db, created using host an db name
    def db_identifier(db)
      "#{db.hostname}/#{db.dbname}"
    end

    # Load the table data
    def load(db, table, id_column, name_column, silent)
      maps = { ids: {}, names: {} }

      db_id = db_identifier(db)
      result = db.exec(load_query(table, id_column, name_column), silent: silent) do |row|
        assign_values(maps, row[id_column], row[name_column], db_id, table)
      end
      raise "Impossible to load the table '#{table}' of db '#{db_id}'" if result.nil?

      @tables_map[db_id][table] = LookUpTableValues.new(maps[:ids], maps[:names]).freeze
    end

    # Create the query to be used for load the table
    def load_query(table, id_column, name_column)
      query = Query.new
      query.fields = [id_column, name_column]
      query.from = table
      query
    end

    # Assign the values to the maps
    def assign_values(maps, id, name, db_id, table)
      id = id.to_i
      name_sym = name.to_sym

      raise "Duplicated id '#{id}' in table '#{table}' of db '#{db_id}'" if maps[:ids].key?(id)
      raise "Duplicated name '#{name}' in table '#{table}' of db '#{db_id}'" if maps[:names].key?(name_sym)

      maps[:ids][id] = name
      maps[:names][name_sym] = id
    end

    # Map for single look-up table id/value association
    class LookUpTableValues
      def initialize(id_map, name_map)
        @id_map = id_map
        @name_map = name_map
      end

      # Return the id corresponding to the given name
      def id(name)
        @name_map[name.to_sym]
      end

      # Return the name corresponding to the given id
      def name(id)
        @id_map[id.to_i]
      end
    end
  end
end
