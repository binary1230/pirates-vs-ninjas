#!/bin/bash

echo "Regenerating autoconf/automake stuff.."

aclocal
autoconf
automake
