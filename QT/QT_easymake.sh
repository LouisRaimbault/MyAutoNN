#!/bin/bash

# ==============================
# 💻 Build Script - Qt Version
# ==============================

SRC_DIR="src"
BUILD_DIR="build"
TARGET="qt_app"

mkdir -p $BUILD_DIR

# Flags Qt5 (manuels comme dans ton message)
CXX=g++
CXXFLAGS="-std=c++17 -Wall -fPIC \
    -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB \
    -I/usr/include/x86_64-linux-gnu/qt5/QtWidgets \
    -I/usr/include/x86_64-linux-gnu/qt5 \
    -I/usr/include/x86_64-linux-gnu/qt5/QtGui \
    -I/usr/include/x86_64-linux-gnu/qt5/QtCore"

LDFLAGS="-lQt5Widgets -lQt5Gui -lQt5Core"

echo "CXXFLAGS=$CXXFLAGS"
echo "LDFLAGS=$LDFLAGS"
echo ""
echo "Compilation des fichiers source..."

# Compilation des .o

$CXX -c $SRC_DIR/Completion_Window.cpp -o $BUILD_DIR/Completion_Window.o $CXXFLAGS
echo "Completion_Window.o ✓"

$CXX -c $SRC_DIR/Modele_Window.cpp -o $BUILD_DIR/Modele_Window.o $CXXFLAGS
echo "Modele_Window.o ✓"

$CXX -c $SRC_DIR/Apply_Modele_Window.cpp -o $BUILD_DIR/Apply_Modele_Window.o $CXXFLAGS
echo "Apply_Modele_Window.o ✓"

$CXX -c $SRC_DIR/Train_Modele_Window.cpp -o $BUILD_DIR/Train_Modele_Window.o $CXXFLAGS
echo "Train_Modele_Window.o ✓"

$CXX -c $SRC_DIR/Welcome_Window.cpp -o $BUILD_DIR/Welcome_Window.o $CXXFLAGS
echo "Welcome_Window.o ✓"

$CXX -c $SRC_DIR/QT_General.cpp -o $BUILD_DIR/QT_General.o $CXXFLAGS
echo "QT_General.o ✓"

$CXX -c $SRC_DIR/main.cpp -o $BUILD_DIR/main.o $CXXFLAGS
echo "main.o ✓"

echo ""
echo "🔗 Edition de liens..."

$CXX \
    $BUILD_DIR/Completion_Window.o \
    $BUILD_DIR/Modele_Window.o \
    $BUILD_DIR/Apply_Modele_Window.o \
    $BUILD_DIR/Train_Modele_Window.o \
    $BUILD_DIR/Welcome_Window.o \
    $BUILD_DIR/QT_General.o \
    $BUILD_DIR/main.o \
    -o $BUILD_DIR/$TARGET \
    $LDFLAGS

echo ""
echo "====================================="
echo "✅ Build terminé : $BUILD_DIR/$TARGET"
echo "====================================="
