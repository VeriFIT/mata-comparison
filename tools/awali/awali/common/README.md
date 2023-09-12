

Files in `awali/common` contains the files that are both meaningly in sttc and dyn layers.

## If both `<name>.cc` and `<name>.hh`
- `<name>.cc` is to be included in sttc layer,or by programs using directly files in sttc.
- `<name>.hh` is  to be included in dyn layer, or by programs that are linked to awalidyn.

## If only `<name>.hh` exists
- Only contains class definition and inline functions
- May safely be included in all cases.

## If only `<name>.incl.hh` exists
- This file is part of another file
- It should not be included directly

## If only `<name>.hxx` exists
- All functions defined in this file are fully templated.
- May safely be included in all cases.

