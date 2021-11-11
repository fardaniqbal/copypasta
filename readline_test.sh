#!/bin/sh
# Run readline on the given input files and diff its output with the input.
# Output should be identical to input for plain text files (ascii or utf8.)
if [ $# -lt 1 ]; then
  echo "Usage: $0 FILE [...]" 1>&2
  exit 1
fi

exit_status=0
for i in "$@"; do
  expected_file=$(mktemp "/tmp/rdln-$(basename "$i")-expected.XXXXXX" 2>&1)
  actual_file=$(mktemp "/tmp/rdln-$(basename "$i")-actual.XXXXXX" 2>&1)

  # Capture the outputs before writing them to temp files.  This ensures
  # that the expected and actual outputs both end with a newline.
  expected=$(sed 's/\r$//' < "$i") # actual output shouldn't have crlf
  actual=$(./readline "$i")
  printf "%s\n" "$expected" > "$expected_file"
  printf "%s\n" "$actual" > "$actual_file"

  diff_out=$(diff -u "$expected_file" "$actual_file")
  diff_status=$?
  rm -f "$expected_file" "$actual_file"
  diff_out=$(printf "%s" "$diff_out" | tr -d '\033') # remove escape codes

  # Trim diff output to 10 lines.
  diff_nline=$(printf "%s\n" "$diff_out" | wc -l)
  if [ $diff_nline -gt 10 ]; then
    diff_out=$(printf "%s" "$diff_out" | head -n 9)
    diff_out=$(printf "%s\n(...%d line(s) omitted...)" "$diff_out" \
               $(($diff_nline - 9)))
  fi
  # Show results.
  [ -z "$diff_out" ] || printf "%s\n" "$diff_out"
  printf "%-60s status=%d\n" "diff expected vs actual: $i:" $diff_status
  [ $diff_status = 0 ] || exit_status=1
done
exit $exit_status
