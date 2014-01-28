## 6.S096 Final Project Makefile header
# If you have further questions about anything in the Makefile,
# first take a look at the manual and then ask on Piazza.
# Manual: http://www.gnu.org/software/make/manual/make.html

## First item: dummy "all" rule so it's the first target
## our Makefile sees. By default, running just 'make' will
## be the same as 'make all'; for the actual definition of
## 'all', look in all_tail.mk.
all : 

## Here we'll define the important build locations.
DEV_DIR := .
BUILD_DIR := $(DEV_DIR)/build
INSTALL_DIR := $(DEV_DIR)/install
GTEST_DIR := $(DEV_DIR)/third_party/gtest

## The compilers and programs to use
SHELL := /bin/sh
CC := gcc
# If you're a Mac user and only have clang,
# you'll want to change CXX and LD to clang++.
CXX := g++
LD := g++
CP := cp -r
RSYNC := rsync -iCau --exclude='\.*' --delete
AR := ar
RM := rm -f
ZIP := zip
MKDIR := mkdir -p

## Directories to include headers from
INCLUDE_FLAGS := -I$(INSTALL_DIR)/include \
								 -I$(GTEST_DIR)/include \
								 -I$(GTEST_DIR)

## Warning flags to use during compilation
FLAGS := -m64 -Wall -Wextra -Wshadow -Werror -pedantic
# Use the C99 standard
CFLAGS := -std=c99 $(FLAGS)
# Use the C++11 standard and warn on violations of Meyers' "Effective C++"
CXXFLAGS := -std=c++11 -Weffc++ $(FLAGS)
# Flags for the linker; link to math and pthread (required for gtest)
LDFLAGS := -L$(INSTALL_DIR)/lib -L$(GTEST_DIR)/lib -lm -lpthread

## Turn on debugging symbols and disable optimizations when running 'make'
DEBUG_FLAGS := -g -O0 -D _DEBUG
## Enable optimizations and turn off asserts when running 'make release'
RELEASE_FLAGS := -O2 -D NDEBUG

## We'll put any extra things that should be cleaned up
## after running 'make clean' here (the copied include/
## files, for example)
ARTIFACTS := 

