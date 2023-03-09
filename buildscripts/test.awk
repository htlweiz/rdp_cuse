BEGIN {
RS=""
}
/.*Failed.*/ { print $0}
END {
  print a
  print b
}
