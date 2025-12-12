![Windows Build](https://github.com/ronak-2004/Autocomplete_Dictionary_Engine/actions/workflows/cpp-windows.yml/badge.svg)

# Autocomplete Trie (C++)

A small command-line autocomplete suggestion tool using a Trie data structure.
Supports insert, search, delete, list suggestions sorted by frequency, and load from file.

## Build
g++ -std=c++17 -O2 src/autocomplete_trie.cpp -o autocomplete

## Usage
./autocomplete data/sample_words.txt
Commands: load, add, query, delete, search, help, exit
