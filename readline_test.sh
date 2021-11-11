#!/bin/sh
# Run getline on the given file and diff its output with cat.  Output should be
# identical for plain text files (ascii/utf8.)
if [ $# -ne 1 ]; then
  echo "Usage: $0 FILE" 1>&2
  exit 1
fi

expected_outfile=$(mktemp "/tmp/rdln-$(basename "$1")-expected.XXXXXX" 2>&1)
actual_outfile=$(mktemp "/tmp/rdln-$(basename "$1")-actual.XXXXXX" 2>&1)
cat "$1" > "$expected_outfile"
./readline "$1" > "$actual_outfile"
diff -u "$expected_outfile" "$actual_outfile"
status=$?
rm -f "$expected_outfile" "$actual_outfile"
printf "%-60s status=%d\n" "diff readline vs cat: $1:" $status
exit $status
