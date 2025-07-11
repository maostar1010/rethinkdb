// Copyright 2010-2014 RethinkDB, all rights reserved.
#ifndef CLUSTERING_ADMINISTRATION_TABLES_DB_CONFIG_HPP_
#define CLUSTERING_ADMINISTRATION_TABLES_DB_CONFIG_HPP_

#include <memory>
#include <string>
#include <vector>

#include "clustering/administration/tables/database_metadata.hpp"
#include "containers/uuid.hpp"
#include "rdb_protocol/artificial_table/caching_cfeed_backend.hpp"
#include "rpc/semilattice/view.hpp"

/* This is public because it's used by `real_reql_cluster_interface_t`. It's kind of a
trivial function right now, but it will become non-trivial if we ever add more
configuration to databases. */
ql::datum_t convert_db_config_and_name_to_datum(
        name_string_t db_name,
        namespace_id_t id);

class real_reql_cluster_interface_t;

class db_config_artificial_table_backend_t :
    public caching_cfeed_artificial_table_backend_t
{
public:
    db_config_artificial_table_backend_t(
            rdb_context_t *rdb_context,
            lifetime_t<name_resolver_t const &> name_resolver,
            std::shared_ptr< semilattice_readwrite_view_t<
            databases_semilattice_metadata_t> > _database_sl_view,
            real_reql_cluster_interface_t *_reql_cluster_interface);
    ~db_config_artificial_table_backend_t();

    std::string get_primary_key_name();

    bool read_all_rows_as_vector(
            auth::user_context_t const &user_context,
            signal_t *interruptor_on_caller,
            std::vector<ql::datum_t> *rows_out,
            admin_err_t *error_out);

    bool read_row(
            auth::user_context_t const &user_context,
            ql::datum_t primary_key,
            signal_t *interruptor_on_caller,
            ql::datum_t *row_out,
            admin_err_t *error_out);

    bool write_row(
            auth::user_context_t const &user_context,
            ql::datum_t primary_key,
            bool pkey_is_autogenerated,
            ql::datum_t *new_value_inout,
            signal_t *interruptor_on_caller,
            admin_err_t *error_out);

private:
    rdb_context_t *rdb_context;
    std::shared_ptr<semilattice_readwrite_view_t<
        databases_semilattice_metadata_t> > database_sl_view;
    semilattice_read_view_t<databases_semilattice_metadata_t>::subscription_t subs;
    real_reql_cluster_interface_t *reql_cluster_interface;
};

#endif // CLUSTERING_ADMINISTRATION_TABLES_DB_CONFIG_HPP_

