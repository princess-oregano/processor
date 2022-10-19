ASM := asm
CPU := cpu

SRCDIR = src
OBJDIR = obj

SRCDIR_ASM := $(addprefix $(SRCDIR)/, $(ASM))
SRCDIR_CPU := $(addprefix $(SRCDIR)/, $(CPU))
OBJDIR_ASM := $(addprefix $(OBJDIR)/, $(ASM))
OBJDIR_CPU := $(addprefix $(OBJDIR)/, $(CPU))

SRC_ASM := main.cpp ../text.cpp text2code.cpp ../stack.cpp ../error.cpp
SRC_CPU := main.cpp process.cpp ../text.cpp ../stack.cpp ../error.cpp
OBJ_ASM := $(addprefix $(OBJDIR_ASM)/, $(SRC_ASM:.cpp=.o))
OBJ_CPU := $(addprefix $(OBJDIR_CPU)/, $(SRC_CPU:.cpp=.o))

CXX := g++
CXXFLAGS := -O3 -g -std=c++14 -fmax-errors=100 -Wall -Wextra                  \
	    -Weffc++ -Waggressive-loop-optimizations -Wc++0x-compat           \
	    -Wc++11-compat -Wc++14-compat -Wcast-align -Wcast-qual            \
	    -Wchar-subscripts -Wconditionally-supported -Wconversion          \
	    -Wctor-dtor-privacy -Wempty-body -Wfloat-equal                    \
	    -Wformat-nonliteral -Wformat-security -Wformat-signedness         \
	    -Wformat=2 -Winline -Wlarger-than=16384 -Wlogical-op              \
	    -Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd           \
	    -Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls   \
	    -Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192        \
	    -Wstrict-null-sentinel -Wstrict-overflow=2                        \
	    -Wsuggest-attribute=noreturn -Wsuggest-final-methods              \
	    -Wsuggest-final-types -Wsuggest-override -Wswitch-default         \
	    -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused     \
	    -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix              \
	    -Wno-missing-field-initializers -Wno-narrowing                    \
	    -Wno-old-style-cast -Wno-varargs -fcheck-new                      \
	    -fsized-deallocation -fstack-check -fstack-protector              \
	    -fstrict-overflow -flto-odr-type-merging                          \
	    -fno-omit-frame-pointer -fsanitize=alignment                      \
            -fsanitize=address -fsanitize=bool -fsanitize=bounds              \
	    -fsanitize=enum -fsanitize=float-cast-overflow                    \
	    -fsanitize=float-divide-by-zero                                   \
	    -fsanitize=integer-divide-by-zero -fsanitize=leak                 \
	    -fsanitize=nonnull-attribute -fsanitize=null                      \
	    -fsanitize=object-size -fsanitize=return                          \
	    -fsanitize=returns-nonnull-attribute -fsanitize=shift             \
	    -fsanitize=signed-integer-overflow                                \
	    -fsanitize=undefined -fsanitize=unreachable                       \
	    -fsanitize=vla-bound -fsanitize=vptr -fPIE -lm -pie
.SILENT:
all: out run

run:
	printf "%s\n" "Assembling..."
	./$(ASM) test.txt
	printf "%s\n" "Running..."
	./$(CPU) test.mur
	printf "%s\n" "Finished."

out: $(OBJDIR) $(OBJ_ASM) $(OBJ_CPU)
	printf "%s\n" "Linking..."
	$(CXX) $(OBJ_ASM) -o $(ASM) $(CXXFLAGS)
	$(CXX) $(OBJ_CPU) -o $(CPU) $(CXXFLAGS)

$(OBJDIR_ASM)/%.o: $(SRCDIR_ASM)/%.cpp
	printf "%s\n" "Compiling $@..."
	$(CXX) -c $^ -o  $@ $(CXXFLAGS)

$(OBJDIR_CPU)/%.o: $(SRCDIR_CPU)/%.cpp
	printf "%s\n" "Compiling $@..."
	$(CXX) -c $^ -o  $@ $(CXXFLAGS)

$(OBJDIR):
	printf "%s\n" "Making $@/ directory..."
	mkdir $@
	mkdir $(OBJDIR_ASM)
	mkdir $(OBJDIR_CPU)

clean:
	printf "%s\n" "Removing $(OBJDIR)/ directory..."
	rm -rf $(OBJDIR)
	printf "%s\n" "Done."

distclean:
	printf "%s\n" "Removing built files..."
	rm -rf $(OBJDIR)
	rm $(ASM)
	rm $(CPU)
	printf "%s\n" "Done."

