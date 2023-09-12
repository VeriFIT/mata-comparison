FOREACH(f 
  "ClassicalTransformations1"
  "ClassicalTransformations2"
  "Edition1"
  "Edition2"
  "Edition3"
  "RationalExpression"
) # Put jupyter file names in the previous list without the ".ipynb"

#Command to generate html from ipynb
  add_custom_target("gs-py-${f}"
    COMMAND "${NBCONVERT}" "--to=html" "--output-dir=${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${f}.ipynb")
  add_dependencies(gs "gs-py-${f}")


#Command to upload html and ipyn files to files.awali.org
  add_custom_target("gs-py-upload-${f}"
    COMMAND "${SCRIPT_DIR}/upload_gs.sh" "${VERSION_MAJOR}.${VERSION_MINOR}" "${CMAKE_CURRENT_SOURCE_DIR}/${f}.ipynb" "${CMAKE_CURRENT_BINARY_DIR}/${f}.html"
    DEPENDS "gs-py-${f}"
  )
  add_dependencies("upload-gs" "gs-py-upload-${f}")
ENDFOREACH(f)
