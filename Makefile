#!/usr/bin/env make

#
# (WIP) Makefile for Py3K compyler.
#
# TODO(basil-conto):
#  * Add 'help' target
#    - Document CL options
#  * Check Coco/R version
#  * Prettify?
#

# Directories
bindir ::= bin/
cocdir ::= coco/
docdir ::= doc/
gendir ::= gen/
srcdir ::= src/
frmdir ::= $(srcdir)frames/

# Coco/R URLs
prefix ::= http://ssw.jku.at/Coco/					# Website URL prefix
manual ::= $(prefix)Doc/UserManual.pdf      # User manual URL
cocurl ::= $(prefix)CPP/CocoSourcesCPP.zip  # Coco/R zip remote URL
coczip ::= $(cocdir)$(notdir $(cocurl))     # Coco/R zip local path
cocoex ::= $(bindir)coco                    # Coco/R executable

# ATG namespace (TBD)
namesp ::= Namespace

# Language standard defaults to c++11; pass STD=gnu for gnu++11
ifeq ($(STD), gnu)
	override CXXFLAGS += -std=gnu++11
else
	override CXXFLAGS += -std=c++11
endif

# Enable compilation warnings via WARNINGS=on
ifeq ($(WARNINGS), on)
	override CXXFLAGS += -pedantic -Wall
endif

.PHONY: all doc coco clean

all: doc coco

# Pull user manual if remote is newer
doc: $(docdir)
	wget -NP $< $(manual)

# Create compyler compiler
# compyler: coco # ...
# 	$(cocoex) -frames $(frmdir) -o $(gendir) -namespace $(namesp) S(srcdir)/grammar/$(namesp).ATG
# 	$(CXX) -o $(bindir)cpc src/Main.cpp $(gendir)*.cpp

# Pull and build Coco/R source if remote is newer
coco: $(cocdir) | $(frmdir) $(bindir)
	wget -NP $< $(cocurl)                   # Pull Coco/R zip
	unzip -qud $< $(coczip) *.cpp *.h       # Extract source
	unzip -qud $(frmdir) $(coczip) *.frame	# Extract frames
	$(LINK.cpp) -o $(cocoex) $<*.cpp        # Build Coco/R

# Remove binary, Coco/R, documentation and frame directories
clean:
	$(RM) -r $(bindir) $(cocdir) $(docdir) $(frmdir)

# Make parent directories corresponding to dependencies with trailing slashes
%/:; mkdir -p $@
