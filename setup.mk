#############################################################################
# File       [ setup.mk ]
# Author     [ littleshamoo ]
# Synopsis   [ package and library setup and customization ]
# Date       [ 2010/11/30 ]
#############################################################################


### modes ###################################################################
# MODES                - available modes
# MODE                 - current mode
# MODE_<MODE>_INCS     - include locations for MODE
# MODE_<MODE>_LD_PATHS - link paths for MODE
# MODE_<MODE>_LD_LIBS  - link libraries for MODE
# MODE_<MODE>_<FLAG>   - compiler flags for MODE
#############################################################################
MODES = opt dbg
MODE = dbg
MODE_opt_C_FLAGS = -O2
MODE_dbg_C_FLAGS = -g
MODE_$(MODE)_LD_PATHS = $(LIB_TOP)/$(MODE)

### packages ################################################################
# PKGS                           - packages to be compiled
# PKG_<PKG>_DEPS                 - package dependencies
# PKG_<PKG>_MAINS                - package main function files
# PKG_<PKG>_INCS                 - package include locations
# PKG_<PKG>_LD_PATHS             - package link locations
# PKG_<PKG>_LD_LIBS              - package link libraries
# PKG_<PKG>_<FLAGS>              - package compiler flags
# PKG_<PKG>_MODE_<MODE>_INCS     - package include locations for MODE
# PKG_<PKG>_MODE_<MODE>_LD_PATHS - package link locations for MODE
# PKG_<PKG>_MODE_<MODE>_LD_LIBS  - package link libraries for MODE
# PKG_<PKG>_MODE_<MODE>_<FLAGS>  - package compiler flags for MODE
#############################################################################
PKGS = atpg cli cmd interface core 

PKG_atpg_MAINS = main.cpp
## mains below for testing 
#PKG_interface_MAINS = netlist_test.cpp
#PKG_core_MAINS = logic_test.cpp
PKG_cli_MAINS = cli_test.cpp

PKG_atpg_DEP = cli interface 
PKG_cli_DEP = cmd 
PKG_interface_DEP = core

### main functions ##########################################################
# MAIN_<PKG>/<MAIN>_BIN                  - binary name
# MAIN_<PKG>/<MAIN>_LD_PATHS             - link paths
# MAIN_<PKG>/<MAIN>_LD_LIBS              - link libraries
# MAIN_<PKG>/<MAIN>_LD_FLAGS             - link flags
# MAIN_<PKG>/<MAIN>_MODE_<MODE>_LD_PATHS - link paths for MODE
# MAIN_<PKG>/<MAIN>_MODE_<MODE>_LD_LIBS  - link libraries for MODE
# MAIN_<PKG>/<MAIN>_MODE_<MODE>_LD_FLAGS - link flags for MODE
#############################################################################
MAIN_atpg/main.cpp_BIN = atpg
MAIN_atpg/main.cpp_LD_LIBS = cli cmd interface core

## bins below for testing 
#MAIN_interface/netlist_test.cpp = nl_test
#MAIN_core/logic_test.cpp = logic_test
MAIN_cli/cli_test.cpp_BIN = cli_test 
MAIN_cli/cli_test.cpp_LD_LIBS = cmd

### libraries ###############################################################
# STA_LIBS        - static archived libraries
# DYN_LIBS        - dynamic loaded shared libraries
# LIB_$(LIB)_PKGS - library packages
#############################################################################

STA_LIBS = cli cmd interface core
LIB_cli_PKGS = cli
LIB_cmd_PKGS = cmd
LIB_interface_PKGS = interface
LIB_core_PKGS = core 
