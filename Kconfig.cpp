#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/raw_ostream.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace clang;
using namespace std;

namespace {

struct Kconfigs : public PPCallbacks {

  CompilerInstance &CI;
  Rewriter TheRewriter;

  Kconfigs(CompilerInstance &CI) : CI(CI) {
    TheRewriter = Rewriter(CI.getSourceManager(), CI.getLangOpts());
  }

  void Ifdef(SourceLocation Loc, const Token &MacroNameTok,
             const MacroDefinition &MD) {

    // Prev token (#) loc
    SourceLocation Prev = Loc.getLocWithOffset(-1);

    // Next Token - ifdef "CONFIG_XYZ"
    Optional<Token> NextToken =
        Lexer::findNextToken(Loc, CI.getSourceManager(), CI.getLangOpts());

    IdentifierInfo *II = MacroNameTok.getIdentifierInfo();

    string replace;
    replace+= "if( ";
    replace+= II->getName();
    replace+= " ){";

    TheRewriter.ReplaceText(
        Prev, MacroNameTok.getLength() + NextToken->getLength() - 1, replace);
  }

  void Else(SourceLocation Loc, SourceLocation IfLoc) {

    SourceLocation Prev = Loc.getLocWithOffset(-1);

    TheRewriter.ReplaceText(Prev, 5, "} else{");
  }

  void Endif(SourceLocation Loc, SourceLocation IfLoc){

    SourceLocation Prev = Loc.getLocWithOffset(-1);

    TheRewriter.ReplaceText(Prev, 6, "}");

    const RewriteBuffer *RewriteBuf =
        TheRewriter.getRewriteBufferFor(CI.getSourceManager().getMainFileID());
    llvm::outs() << string(RewriteBuf->begin(), RewriteBuf->end());
  }
};

class KconfigAction : public PluginASTAction {
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {
    // llvm::outs() << "In CreateASTConsumer";
    return llvm::make_unique<ASTConsumer>();
  }

  bool BeginSourceFileAction(CompilerInstance &CI) override;

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override;
};

bool KconfigAction::BeginSourceFileAction(CompilerInstance &CI) {
  // llvm::outs() << "In KconfigAction::BeginSourceFileAction";
  Preprocessor &PP = CI.getPreprocessor();
  PP.addPPCallbacks(llvm::make_unique<Kconfigs>(CI));
  return true;
}

bool KconfigAction::ParseArgs(const CompilerInstance &,
                          const std::vector<std::string> &args) {
  // llvm::outs() << "In KconfigAction::ParseArgs";
  for (unsigned i = 0, e = args.size(); i != e; ++i)
    llvm::outs() << "Received arg: " << args[i] << "\n";
  return true;
}

} // anonymous namespace

static FrontendPluginRegistry::Add<KconfigAction>
X("kconfig", "refactores kconfig ifdefs");
