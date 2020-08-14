#
# MIT License
#
# Copyright(c) 2011-2020 The Maintainers of Nanvix
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

# Builds everything.
all: all-hello all-shared-variable all-producer-consumer

# Cleans up build objects.
clean: clean-hello clean-shared-variable clean-producer-consumer

# Cleans up everything.
distclean: distclean-hello distclean-shared-variable distclean-producer-consumer

#===============================================================================
# Hello
#===============================================================================

# Builds the app.
all-hello:
	$(MAKE) -C hello all

# Cleans up build objects.
clean-hello:
	$(MAKE) -C hello clean

# Cleans up everything.
distclean-hello:
	$(MAKE) -C hello distclean

#===============================================================================
# Global Variable
#===============================================================================

# Builds the app.
all-shared-variable:
	$(MAKE) -C shared-variable all

# Cleans up build objects.
clean-shared-variable:
	$(MAKE) -C shared-variable clean

# Cleans up everything.
distclean-shared-variable:
	$(MAKE) -C shared-variable distclean

#===============================================================================
# Producer Consumer Problem
#===============================================================================

# Builds the app.
all-producer-consumer:
	$(MAKE) -C producer-consumer all

# Cleans up build objects.
clean-producer-consumer:
	$(MAKE) -C producer-consumer clean

# Cleans up everything.
distclean-producer-consumer:
	$(MAKE) -C producer-consumer distclean
