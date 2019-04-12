export BD=~/llvm/llvm-project/build

rm -f $BD/lib/Kconfig.dylib

# (cd $BD && cmake -DLLVM_ENABLE_PROJECTS=clang -DLLVM_TARGETS_TO_BUILD=X86 -G "Unix Makefiles" ../llvm)

(cd $BD && make Kconfig )

echo "=====\n"

(cd $BD/.. && clang -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS \
          -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -D_GNU_SOURCE \
          -I$BD/tools/clang/include -I$BD/include -Iinclude \
          $BD/../clang/examples/Kconfig/sample/kconfigtest.c -fsyntax-only \
          -Xclang -load -Xclang $BD/lib/Kconfig.dylib -Xclang \
          -plugin -Xclang kconfig)

echo "\n====="
