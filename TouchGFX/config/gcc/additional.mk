# HMI TouchGFX extended Makefile configuration
#
# Include this file in Makefile with
# `include $(application_path)/TouchGFX/config/gcc/additional.mk`
# before the `# Start converting paths` line of the `gcc/Makefile`
# and before the `` line of `TouchGFX/simulator/gcc/Makefile`
#
# Variables available:
# 	include_paths
#	c_source_files
#	cpp_source_files

ADDITIONAL_INCLUDE_PATHS := ../Tools
ADDITIONAL_SOURCES := ../Tools/TimeSpan.cpp
ADDITIONAL_LIBRARY_PATHS :=
ADDITIONAL_LIBRARIES :=