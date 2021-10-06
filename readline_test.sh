#!/bin/sh
# Run getline on the given file and diff its output with cat.  Output should be
# identical for plain text files (ascii/utf8.)
if [ $# -ne 1 ]; then
  echo "Usage: $0 FILE" 1>&2
  exit 1
fi

expected_outfile=$(mktemp "/tmp/$(basename "$0")-XXXXXX.diff" 2>&1)
actual_outfile=$(mktemp "/tmp/$(basename "$0")-XXXXXX.diff" 2>&1)
cat "$1" > "$expected_outfile"
./readline "$1" > "$actual_outfile"
diff "$expected_outfile" "$actual_outfile"
status=$?
rm -f "$expected_outfile" "$actual_outfile"
echo "diff exited with code $status"
exit $status
