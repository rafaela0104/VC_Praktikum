set(GLM_VERSION "0.9.9")
set(GLM_INCLUDE_DIRS "/home/rafaela/Uni/Semester4/VC/VC_Praktikum/OpenGL_Praktikum/libs/glm-master")

if (NOT CMAKE_VERSION VERSION_LESS "3.0")
    include("${CMAKE_CURRENT_LIST_DIR}/glmTargets.cmake")
endif()
