# Alternative GNU Make project makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

.PHONY: clean prebuild

SHELLTYPE := posix
ifeq (.exe,$(findstring .exe,$(ComSpec)))
	SHELLTYPE := msdos
endif

# Configurations
# #############################################

RESCOMP = windres
PCH = src/pch.hpp
PCH_PLACEHOLDER = $(OBJDIR)/$(notdir $(PCH))
GCH = $(PCH_PLACEHOLDER).gch
INCLUDES += -Iexternal/include
FORCE_INCLUDE +=
ALL_CPPFLAGS += $(CPPFLAGS) -MD -MP $(DEFINES) $(INCLUDES)
ALL_RESFLAGS += $(RESFLAGS) $(DEFINES) $(INCLUDES)
LIBS += -lglfw3 -lsoil2 -lspdlog -luser32 -lgdi32 -lopengl32
LDDEPS +=
LINKCMD = $(CXX) -o "$@" $(OBJECTS) $(RESOURCES) $(ALL_LDFLAGS) $(LIBS)
define PREBUILDCMDS
endef
define PRELINKCMDS
endef
define POSTBUILDCMDS
endef

ifeq ($(config),debug)
TARGETDIR = bin/Debug
TARGET = $(TARGETDIR)/OpenGLPlayground.exe
OBJDIR = obj/Debug
DEFINES += -DSPDLOG_COMPILED_LIB -DDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -g
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -g -std=c++17
ALL_LDFLAGS += $(LDFLAGS) -Lexternal/lib

else ifeq ($(config),release)
TARGETDIR = bin/Release
TARGET = $(TARGETDIR)/OpenGLPlayground.exe
OBJDIR = obj/Release
DEFINES += -DSPDLOG_COMPILED_LIB -DNDEBUG
ALL_CFLAGS += $(CFLAGS) $(ALL_CPPFLAGS) -O2
ALL_CXXFLAGS += $(CXXFLAGS) $(ALL_CPPFLAGS) -O2 -std=c++17
ALL_LDFLAGS += $(LDFLAGS) -Lexternal/lib -s

endif

# Per File Configurations
# #############################################


# File sets
# #############################################

GENERATED :=
OBJECTS :=

GENERATED += $(OBJDIR)/Camera.o
GENERATED += $(OBJDIR)/Chunk.o
GENERATED += $(OBJDIR)/Context.o
GENERATED += $(OBJDIR)/Debug.o
GENERATED += $(OBJDIR)/FlyCamera.o
GENERATED += $(OBJDIR)/GrassRenderer.o
GENERATED += $(OBJDIR)/Input.o
GENERATED += $(OBJDIR)/Light.o
GENERATED += $(OBJDIR)/Loader.o
GENERATED += $(OBJDIR)/Log.o
GENERATED += $(OBJDIR)/OrbitCamera.o
GENERATED += $(OBJDIR)/QuadTree.o
GENERATED += $(OBJDIR)/Renderable.o
GENERATED += $(OBJDIR)/Shader.o
GENERATED += $(OBJDIR)/Skybox.o
GENERATED += $(OBJDIR)/Terrain.o
GENERATED += $(OBJDIR)/gl.o
GENERATED += $(OBJDIR)/main.o
OBJECTS += $(OBJDIR)/Camera.o
OBJECTS += $(OBJDIR)/Chunk.o
OBJECTS += $(OBJDIR)/Context.o
OBJECTS += $(OBJDIR)/Debug.o
OBJECTS += $(OBJDIR)/FlyCamera.o
OBJECTS += $(OBJDIR)/GrassRenderer.o
OBJECTS += $(OBJDIR)/Input.o
OBJECTS += $(OBJDIR)/Light.o
OBJECTS += $(OBJDIR)/Loader.o
OBJECTS += $(OBJDIR)/Log.o
OBJECTS += $(OBJDIR)/OrbitCamera.o
OBJECTS += $(OBJDIR)/QuadTree.o
OBJECTS += $(OBJDIR)/Renderable.o
OBJECTS += $(OBJDIR)/Shader.o
OBJECTS += $(OBJDIR)/Skybox.o
OBJECTS += $(OBJDIR)/Terrain.o
OBJECTS += $(OBJDIR)/gl.o
OBJECTS += $(OBJDIR)/main.o

# Rules
# #############################################

all: $(TARGET)
	@:

$(TARGET): $(GENERATED) $(OBJECTS) $(LDDEPS) | $(TARGETDIR)
	$(PRELINKCMDS)
	@echo Linking OpenGLPlayground
	$(SILENT) $(LINKCMD)
	$(POSTBUILDCMDS)

$(TARGETDIR):
	@echo Creating $(TARGETDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(TARGETDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(TARGETDIR))
endif

$(OBJDIR):
	@echo Creating $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) mkdir -p $(OBJDIR)
else
	$(SILENT) mkdir $(subst /,\\,$(OBJDIR))
endif

clean:
	@echo Cleaning OpenGLPlayground
ifeq (posix,$(SHELLTYPE))
	$(SILENT) rm -f  $(TARGET)
	$(SILENT) rm -rf $(GENERATED)
	$(SILENT) rm -rf $(OBJDIR)
else
	$(SILENT) if exist $(subst /,\\,$(TARGET)) del $(subst /,\\,$(TARGET))
	$(SILENT) if exist $(subst /,\\,$(GENERATED)) del /s /q $(subst /,\\,$(GENERATED))
	$(SILENT) if exist $(subst /,\\,$(OBJDIR)) rmdir /s /q $(subst /,\\,$(OBJDIR))
endif

prebuild: | $(OBJDIR)
	$(PREBUILDCMDS)

ifneq (,$(PCH))
$(OBJECTS): $(GCH) | $(PCH_PLACEHOLDER)
$(GCH): $(PCH) | prebuild
	@echo $(notdir $<)
	$(SILENT) $(CXX) -x c++-header $(ALL_CXXFLAGS) -o "$@" -MF "$(@:%.gch=%.d)" -c "$<"
$(PCH_PLACEHOLDER): $(GCH) | $(OBJDIR)
ifeq (posix,$(SHELLTYPE))
	$(SILENT) touch "$@"
else
	$(SILENT) echo $null >> "$@"
endif
else
$(OBJECTS): | prebuild
endif


# File Rules
# #############################################

$(OBJDIR)/gl.o: external/src/glad/gl.c
	@echo "$(notdir $<)"
	$(SILENT) $(CC) -include $(PCH_PLACEHOLDER) $(ALL_CFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Camera.o: src/Camera.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Chunk.o: src/Chunk.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Context.o: src/Context.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Debug.o: src/Debug.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/FlyCamera.o: src/FlyCamera.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/GrassRenderer.o: src/GrassRenderer.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Input.o: src/Input.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Light.o: src/Light.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Loader.o: src/Loader.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Log.o: src/Log.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/OrbitCamera.o: src/OrbitCamera.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/QuadTree.o: src/QuadTree.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Renderable.o: src/Renderable.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Shader.o: src/Shader.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Skybox.o: src/Skybox.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/Terrain.o: src/Terrain.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"
$(OBJDIR)/main.o: src/main.cpp
	@echo "$(notdir $<)"
	$(SILENT) $(CXX) -include $(PCH_PLACEHOLDER) $(ALL_CXXFLAGS) $(FORCE_INCLUDE) -o "$@" -MF "$(@:%.o=%.d)" -c "$<"

-include $(OBJECTS:%.o=%.d)
ifneq (,$(PCH))
  -include $(PCH_PLACEHOLDER).d
endif