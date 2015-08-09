#!/bin/sh
GAMECODE_DIR="$PROJECT_DIR/../../../GameCode"
find $GAMECODE_DIR -type d -exec touch "{}" \+
