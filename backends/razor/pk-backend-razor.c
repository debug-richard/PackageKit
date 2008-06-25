/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2008 Richard Hughes <richard@hughsie.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <gmodule.h>
#include <glib.h>
#include <string.h>
#include <pk-backend.h>
#include <pk-backend-internal.h>
#include <pk-debug.h>
#include <pk-package-ids.h>
#include <pk-enum.h>

#include <razor/razor.h>

/**
 * backend_initialize:
 */
static void
backend_initialize (PkBackend *backend)
{
}

/**
 * backend_destroy:
 */
static void
backend_destroy (PkBackend *backend)
{
}


static gboolean
backend_refresh_cache_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return TRUE;
}

/**
 * backend_refresh_cache:
 */
static void
backend_refresh_cache (PkBackend *backend, gboolean force)
{
	pk_backend_set_status (backend, PK_STATUS_ENUM_REFRESH_CACHE);
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_refresh_cache_thread);
}

static gboolean
backend_search_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return FALSE;
}

static void
backend_search_name (PkBackend *backend, PkFilterEnum filters, const gchar *search)
{
	pk_backend_set_status (backend, PK_STATUS_ENUM_QUERY);
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_search_thread);
}

/**
 * backend_search_description:
 */
static void
backend_search_description (PkBackend *backend, PkFilterEnum filters, const gchar *search)
{
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_search_thread);
}

static void
backend_search_group (PkBackend *backend, PkFilterEnum filters, const gchar *search)
{
	pk_backend_set_status (backend, PK_STATUS_ENUM_QUERY);
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_search_thread);
}


static gboolean
backend_install_packages_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return FALSE;
}

static void
backend_install_packages (PkBackend *backend, gchar **package_ids)
{
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_set_status (backend, PK_STATUS_ENUM_INSTALL);
	pk_backend_thread_create (backend, backend_install_packages_thread);
}

static gboolean
backend_remove_packages_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return FALSE;
}

static void
backend_remove_packages (PkBackend *backend, gchar **package_ids, gboolean allow_deps, gboolean autoremove)
{
	pk_backend_set_status (backend, PK_STATUS_ENUM_REMOVE);
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_remove_packages_thread);
}

/**
 * backend_get_filters:
 */
static PkFilterEnum
backend_get_filters (PkBackend *backend)
{
	return (PK_FILTER_ENUM_INSTALLED |
		PK_FILTER_ENUM_DEVELOPMENT |
		PK_FILTER_ENUM_GUI);
}


static gboolean
backend_update_system_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return FALSE;
}

static void
backend_update_system (PkBackend *backend)
{
	pk_backend_set_status (backend, PK_STATUS_ENUM_UPDATE);
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_update_system_thread);
}

/**
 * backend_update_package:
 */
static gboolean
backend_update_package_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return FALSE;
}

static void
backend_update_packages (PkBackend *backend, gchar **package_ids)
{
	pk_backend_set_status (backend, PK_STATUS_ENUM_UPDATE);
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_update_package_thread);
}

static gboolean
backend_get_updates_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return TRUE;
}

static void
backend_get_updates (PkBackend *backend, PkFilterEnum filters)
{
	pk_backend_set_status (backend, PK_STATUS_ENUM_UPDATE);
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_get_updates_thread);
}

/**
 * backend_get_groups:
 */
static PkGroupEnum
backend_get_groups (PkBackend *backend)
{
	return (PK_GROUP_ENUM_COMMUNICATION |
		PK_GROUP_ENUM_PROGRAMMING |
		PK_GROUP_ENUM_GAMES |
		PK_GROUP_ENUM_OTHER |
		PK_GROUP_ENUM_INTERNET |
		PK_GROUP_ENUM_REPOS |
		PK_GROUP_ENUM_MAPS);
}

/**
 * backend_get_details:
 */
static gboolean
backend_get_details_thread (PkBackend *backend)
{
	pk_backend_finished (backend);
	return TRUE;
}

static void
backend_get_details (PkBackend *backend, gchar **package_ids)
{
	pk_backend_set_percentage (backend, PK_BACKEND_PERCENTAGE_INVALID);
	pk_backend_thread_create (backend, backend_get_details_thread);
}

PK_BACKEND_OPTIONS (
	"razor",				/* description */
	"Richard Hughes <richard@hughsie.com>",	/* author */
	backend_initialize,			/* initalize */
	backend_destroy,			/* destroy */
	backend_get_groups,			/* get_groups */
	backend_get_filters,			/* get_filters */
	NULL,					/* cancel */
	NULL,					/* download_packages */
	NULL,					/* get_depends */
	backend_get_details,			/* get_details */
	NULL,					/* get_files */
	NULL,					/* get_packages */
	NULL,					/* get_repo_list */
	NULL,					/* get_requires */
	NULL,					/* get_update_detail */
	backend_get_updates,			/* get_updates */
	NULL,					/* install_files */
	backend_install_packages,		/* install_packages */
	NULL,					/* install_signature */
	backend_refresh_cache,			/* refresh_cache */
	backend_remove_packages,		/* remove_packages */
	NULL,					/* repo_enable */
	NULL,					/* repo_set_data */
	NULL,					/* resolve */
	NULL,					/* rollback */
	backend_search_description,		/* search_details */
	NULL,					/* search_file */
	backend_search_group,			/* search_group */
	backend_search_name,			/* search_name */
	NULL,					/* service_pack */
	backend_update_packages,		/* update_packages */
	backend_update_system,			/* update_system */
	NULL					/* what_provides */
);

