#!/bin/sh
# bld.sh

for F in *.md; do :
  if egrep "<!-- mdtoc-start -->" $F >/dev/null; then :
    # Update doc table of contents (see https://github.com/fordsfords/mdtoc).
    if which mdtoc.pl >/dev/null 2>&1; then LANG=C mdtoc.pl -b "" $F;
    elif [ -x ../mdtoc/mdtoc.pl ]; then LANG=C ../mdtoc/mdtoc.pl -b "" $F;
    else echo "FYI: mdtoc.pl not found; Skipping doc build"; echo ""; fi
  fi
done

rm -rf Linux64
mkdir Linux64

echo "Building code"

gcc -Wall -pthread -g -o Linux64/ev_test -pthread ev.c ev_test.c; if [ $? -ne 0 ]; then exit 1; fi

if [ -f ev_test/x64/Release/ev_test.exe ]; then :
  cp ev_test/x64/Release/ev_test.exe Win64/
fi
