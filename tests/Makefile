########################################################################################
#				 	                EXECUTABLE NAME 	                               #
#				 	                                                                   #
# NOTE: The following conventions are used:                                            #
#                                                                                      #
#       - CU = CUDA related variables                                                  #
#       - CX = C++ related variables                                                   #
########################################################################################

EXE 			:= test_suite
CONTAINER_EXE   := container_suite
OPERATIONS_EXE  := operations_suite
TENSOR_EXE      := tensor_suite
TRAITS_EXE      := traits_suite


########################################################################################
#					                  COMPILERS						                   #
#                                                                                      #					 
# NOTE: Compiling with cuda because GPU support will be added                          #
########################################################################################

HOST_COMPILER	:= clang++
NVCC 			:= nvcc -ccbin $(HOST_COMPILER)
CXX 			:= clang++ 

########################################################################################
#				                    INCLUDE DIRECTORIES 		                       #
########################################################################################

CU_INC 		    := 
CX_INC          :=

########################################################################################
#				   	                   LIBRARIES 						               #
########################################################################################

CU_LIBS 		:= 
CX_LIBS 		:= -lboost_unit_test_framework      

CU_LDIR         :=
CX_LDIR  	    := 

########################################################################################
#					                COMPILER FLAGS 					                   #
#                                                                                      #
# NOTE: To enable compiler warnings, remove -w and replace with :                      #
# 		--compiler-options -Wall                                                       #
########################################################################################

CU_FLAGS        :=
CX_FLAGS 		:= -std=c++11 -w 

DG_FLAGS        := -g
RE_FLAGS        := -O3

TP_FLAGS        := $(CX_FLAGS)

########################################################################################
# 					                TARGET RULES 					                   #
#######################################################################################

.PHONY: all container operations tensor traits

all: debug

debug: CX_FLAGS += $(DG_FLAGS)
debug: build_tests

operations_tests.o: operations_tests.cpp
	$(CXX) $(CU_INC) $(CU_FLAGS) $(CX_INC) $(CX_FLAGS) -o $@ -c $<
	
container_tests.o: container_tests.cpp 
	$(CXX) $(CU_INC) $(CU_FLAGS) $(CX_INC) $(CX_FLAGS) -o $@ -c $<
	
tensor_tests.o: tensor_tests.cpp 
	$(CXX) $(CU_INC) $(CU_FLAGS) $(CX_INC) $(CX_FLAGS) -o $@ -c $<
	
traits_tests.o: traits_tests.cpp 
	$(CXX) $(CU_INC) $(CU_FLAGS) $(CX_INC) $(CX_FLAGS) -o $@ -c $<
	
tests.o: tests.cpp 
	$(CXX) $(CU_INC) $(CU_FLAGS) $(CX_INC) $(CX_FLAGS) -o $@ -c $<

build_tests: container_tests.o tensor_tests.o traits_tests.o operations_tests.o tests.o
	$(CXX) -o $(EXE) $+ $(CU_LDIR) $(CU_LIBS) $(CX_LDIR) $(CX_LIBS)	

container: CX_FLAGS += -DSTAND_ALONE
container: container_tests.o
	$(CXX) -o $(CONTAINER_EXE) $+ $(CU_LDIR) $(CU_LIBS) $(CX_LDIR) $(CX_LIBS)	
	
operations: CX_FLAGS += -DSTAND_ALONE
operations: operations_tests.o
	$(CXX) -o $(OPERATIONS_EXE) $+ $(CU_LDIR) $(CU_LIBS) $(CX_LDIR) $(CX_LIBS)	
	
tensor: CX_FLAGS += -DSTAND_ALONE
tensor: tensor_tests.o
	$(CXX) -o $(TENSOR_EXE) $+ $(CU_LDIR) $(CU_LIBS) $(CX_LDIR) $(CX_LIBS)	
	
traits: CX_FLAGS += -DSTAND_ALONE
traits: traits_tests.o
	$(CXX) -o $(TRAITS_EXE) $+ $(CU_LDIR) $(CU_LIBS) $(CX_LDIR) $(CX_LIBS)	
	
clean:
	rm -rf *.o
	rm -rf $(EXE) 
	rm -rf $(CONTAINER_EXE)
	rm -rf $(OPERATIONS_EXE)
	rm -rf $(TENSOR_EXE)
	rm -rf $(TRAITS_EXE)
