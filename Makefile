#!/usr/bin/env make

#
# (WIP) Makefile for Py3K compyler.
#
# TODO(basil-conto):
#  * Add 'help' target
#    - Document CL options
#

# =========
# Variables
# =========

# Directories
bindir := bin/
cocdir := coco/
docdir := doc/
gendir := gen/
srcdir := src/
frmdir := $(srcdir)frames/

# Coco/R URLs
prefix := http\://ssw.jku.at/Coco/
manual := $(prefix)Doc/UserManual.pdf
cocurl := $(prefix)CPP/CocoSourcesCPP.zip
coczip := $(cocdir)$(notdir $(cocurl))
cocexe := $(bindir)coco
cocver := Jan 02, 2012

# ATG namespace
namesp := Compyler

# ======
# Macros
# ======

# Effectively a ternary conditional operator
ifflag = $(if $(findstring $(2), $(1)), $(3), $(4))

# Inspect output of coco to determine version
define checkversion =
	version="$$($(cocexe) | head -1 | awk -F '[()]' '{print $$2}')"; \
	if [ "$${version}" != "$(cocver)" ]; then \
		echo ''; \
		echo '[ERR] The version of Coco/R has changed!'; \
		echo "Expected $(cocver), but got $${version}"; \
		echo ''; \
		exit 2; \
	fi
endef

# =====
# Flags
# =====

# Language standard defaults to c++11; pass STD=gnu for gnu++11
override CXXFLAGS += $(call ifflag, $(STD), gnu, -std=gnu++11, -std=c++11)

# Enable compilation warnings via WARNINGS=on
override CXXFLAGS += $(call ifflag, $(WARNINGS), on, -pedantic -Wall, -w)

# =======
# Recipes
# =======

.PHONY: all doc coco clean

all: doc coco

# Pull user manual if remote is newer
doc: $(docdir)
	wget -NP $< $(manual)

# Create compyler compiler
compyler:
	$(cocexe) -frames $(frmdir) -o $(gendir) \
	-namespace $(namesp) $(srcdir)/grammar/$(namesp).ATG
	$(CXX) -o $(bindir)cpc src/Main.cpp $(gendir)*.cpp

# Pull and build Coco/R source if remote is newer
coco: $(cocdir) | $(frmdir) $(bindir)
	wget -NP $< $(cocurl)
	unzip -qud $< $(coczip) *.cpp *.h
	unzip -qud $(frmdir) $(coczip) *.frame
	$(LINK.cpp) -o $(cocexe) $<*.cpp
	@ $(checkversion)

# Remove binary, Coco/R, documentation and frame directories
clean:
	$(RM) -r $(bindir) $(cocdir) $(docdir) $(frmdir)

# Make parent directories corresponding to dependencies with trailing slashes
%/:; mkdir -p $@
