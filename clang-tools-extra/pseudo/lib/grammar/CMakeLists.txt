set(LLVM_LINK_COMPONENTS Support)

add_clang_library(clangPseudoGrammar STATIC
  Grammar.cpp
  GrammarBNF.cpp
  LRGraph.cpp
  LRTable.cpp
  LRTableBuild.cpp
  )

