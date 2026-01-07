#!/bin/bash

# ==============================
# 💻 autotorch - Build Script (Optimized)
# ==============================

# 🔧 Compiler avec ccache pour accélérer les recompilations
# (si ccache est installé, sinon il sera ignoré)
CXX="g++"

# ⚙️ Flags de compilation
# -O0 : dev rapide, -O2 : release
# -DNDEBUG : désactive les assertions pour release
CXXFLAGS="-std=c++17 -O2 -Wall \
-Iinclude \
-Iexternal/libtorch/include \
-Iexternal/libtorch/include/torch/csrc/api/include"

# 🧩 Flags de linkage
LDFLAGS="-Lexternal/libtorch/lib \
-ltorch -ltorch_cpu -lc10 -lpthread \
-Wl,-rpath,external/libtorch/lib"

# 🏁 Nom de l’exécutable
TARGET="autotorch"

# 📁 Répertoires
SRC_DIR="src"
BUILD_DIR="build"

mkdir -p $BUILD_DIR

echo "🔧 Compilation avec :"
echo "CXXFLAGS=$CXXFLAGS"
echo "LDFLAGS=$LDFLAGS"
echo

# ===================================
# ⚙️ Compilation des fichiers .cpp → .o
# ===================================

echo "🚀 Compilation des fichiers objets..."

for FILE in one_shot Use_Modele_Inference Use_Modele_Data Use_Modele_Import Export_Modele Get_Config Export_Latex Get_Quality_Modele Modele_Modules Modele_Test Modele_Train Modele_Init Cost_Functions Activation_Functions Usefull_Functions Get_Data Get_Batch main
do
    SRC_FILE="$SRC_DIR/$FILE.cpp"
    OBJ_FILE="$BUILD_DIR/$FILE.o"

    # ⚡️ Compilation incrémentale : ne recompile que si nécessaire
    if [[ "$SRC_FILE" -nt "$OBJ_FILE" ]]; then
        echo "⏳ Compilation de $FILE.cpp..."
        $CXX -c "$SRC_FILE" -o "$OBJ_FILE" $CXXFLAGS || exit 1
    else
        echo "✅ $FILE.o est à jour"
    fi
done

# ===================================
# 🔗 Linking final
# ===================================
echo
echo "🔗 Édition de liens ..."
$CXX \
    $BUILD_DIR/*.o \
    -o $BUILD_DIR/$TARGET \
    $LDFLAGS || exit 1

echo
echo "✅ Build terminé : $BUILD_DIR/$TARGET"