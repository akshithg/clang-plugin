#include "clang/AST/ASTConsumer.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/raw_ostream.h"

using namespace clang;

namespace {

class KconfigAction : public PluginASTAction {

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 llvm::StringRef) override {

    llvm::outs() << "In CreateASTConsumer";

    return llvm::make_unique<ASTConsumer>();
  }

  bool BeginSourceFileAction(CompilerInstance &CI);

  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override;
};

bool KconfigAction::BeginSourceFileAction(CompilerInstance &CI) {

  llvm::outs() << "In KconfigAction::BeginSourceFileAction";

  return true;
}

bool KconfigAction::ParseArgs(const CompilerInstance &,
                          const std::vector<std::string> &args) {

  llvm::outs() << "In KconfigAction::ParseArgs";

  for (unsigned i = 0, e = args.size(); i != e; ++i)
    llvm::outs() << "Received arg: " << args[i] << "\n";
  return true;
}

} // anonymous namespace

static FrontendPluginRegistry::Add<KconfigAction>
X("kconfig", "refactores kconfig ifdefs");
