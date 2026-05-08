#include "Common.h"

String extractFileName(const String& fullPath, bool stripExt) {
  int slash = fullPath.lastIndexOf('/');
  if (slash < 0) slash = fullPath.lastIndexOf('\\');
  String name = (slash < 0) ? fullPath : fullPath.substring(slash + 1);
  if (!stripExt) return name;
  int dot = name.lastIndexOf('.');
  return (dot < 0) ? name : name.substring(0, dot);
}

String lower(const String& s) {
  String r = s; r.toLowerCase(); return r;
}