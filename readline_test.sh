#!/bin/sh
# Run getline on the given file and diff its output with cat.  Output should be
# identical for plain text files (ascii/utf8.)
if [ $# -ne 1 ]; then
  echo "Usage: $0 FILE" 1>&2
  exit 1
fi

expected_outfile=$(mktemp "/tmp/rdln-$(basename "$1")-expected.XXXXXX" 2>&1)
actual_outfile=$(mktemp "/tmp/rdln-$(basename "$1")-actual.XXXXXX" 2>&1)

# Capture the outputs before writing them to temp files.  This ensures that
# the expected and actual outputs both end with a newline.
expected=$(sed 's/\r$//' < "$1") # actual output shouldn't have crlf
actual=$(./readline "$1")
printf "%s\n" "$expected" > "$expected_outfile"
printf "%s\n" "$actual" > "$actual_outfile"

diffout=$(diff -u "$expected_outfile" "$actual_outfile")
status=$?
rm -f "$expected_outfile" "$actual_outfile"

# Remove terminal escape codes from diff output.
[ -z "$diffout" ] || printf "%s\n" "$diffout" | tr -d '\033'
printf "%-60s status=%d\n" "diff expected vs actual: $1:" $status
exit $status
