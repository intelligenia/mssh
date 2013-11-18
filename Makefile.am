SUBDIRS = src

EXTRA_DIST = mssh.schemas mssh_clusters.conf

SCHEMAS_FILE = mssh.schemas

schemadir = $(GCONF_SCHEMA_FILE_DIR)
schema_DATA = $(SCHEMAS_FILE)

if GCONF_SCHEMAS_INSTALL
install-data-local:
	GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(top_builddir)/$(schema_DATA)
endif
