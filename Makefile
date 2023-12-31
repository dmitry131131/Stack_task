CXX = g++
CXXFLAGS =  -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
 			-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported \
  			-Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security \
   			-Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual \
    		-Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
	 		-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods \
	  		-Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef \
	   		-Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix \
	    	-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector \
		 	-fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging \
		  	-fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla \
			-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr
TARGET = main
TEST_TARGET = tes
SourcePrefix = src/
BuildPrefix = build/
BuildFolder = build
TestPrefix = tests/
TestFolder = tests
Include = -Iinclude -IColor_console_output/include

Sources = Stack.cpp Output.cpp Hash.cpp
TestSources = Tests.cpp
#Main = main.cpp

LibObjects = Color_console_output/build/Color_output.o

Source = $(addprefix $(SourcePrefix), $(Sources))
TestSource = $(addprefix $(TestPrefix), $(TestSources))
#MainObject = $(patsubst %.cpp, $(BuildPrefix)%.o, $(Main))

objects = $(patsubst $(SourcePrefix)%.cpp, $(BuildPrefix)%.o, $(Source))
test_objects = $(patsubst $(TestPrefix)%.cpp, $(BuildPrefix)$(TestPrefix)%.o, $(TestSource))

.PHONY : all clean folder test release debug prepare

all : release

release : CXXFLAGS = -O3
release : folder $(objects)
	cd Color_console_output && make

debug : folder $(objects)
	cd Color_console_output && make

test : folder prepare $(objects) $(test_objects) $(TEST_TARGET)

prepare :
	mkdir -p $(BuildPrefix)$(TestFolder)
	cd Color_console_output && make

$(BuildPrefix)%.o : $(SourcePrefix)%.cpp
	@echo [CXX] -c $< -o $@
	@$(CXX) $(CXXFLAGS) $(Include) -c $< -o $@

$(BuildPrefix)$(TestPrefix)%.o : $(TestPrefix)%.cpp
	@echo [CXX] -c $< -o $@
	@$(CXX) $(CXXFLAGS) $(Include) -c $< -o $@

$(TEST_TARGET) : $(objects) $(LibObjects) $(test_objects)
	@echo [CC] $^ -o $@
	@$(CXX) $(CXXFLAGS) $(Include) $^ -o $@

#Useless compilation part for compilling in main
$(TARGET) : $(objects) $(LibObjects) #$(MainObject)
	@echo [CC] $^ -o $@
	@$(CXX) $(CXXFLAGS) $(Include) $^ -o $@

clean :
	rm $(BuildFolder)/*.o
#	rm $(TARGET)
	cd Color_console_output && make clean

folder :
	mkdir -p $(BuildFolder)