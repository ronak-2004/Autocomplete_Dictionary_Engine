#!/usr/bin/env bash
set -e
# build
g++ -std=c++17 -O2 src/autocomplete.cpp -o autocomplete
# run with sample words file
./autocomplete data/sample_words.txt <<'EOF'
query ap
add apex
query ap
search apple
delete apple
search apple
exit
EOF
