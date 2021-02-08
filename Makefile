###
#
# MIT License
#
# Copyright (c) 2021 Erik Kline
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
###

CFLAGS += -Wall -Wpedantic
SHELL := bash

.PHONY: check test
check test: test_mac_to_modified_eui64 \
            test_modified_eui64_to_mac_1 \
            test_modified_eui64_to_mac_2

.PHONY: test_mac_to_modified_eui64
test_mac_to_modified_eui64: mac2iid
	@if [ "`./mac2iid ce:22:4a:08:51:a0`" != "::cc22:4aff:fe08:51a0" ]; \
	then \
	    echo "FAIL: $(@)" 1>&2; \
	else \
	    echo "PASS: $(@)" 1>&2; \
	fi

.PHONY: test_modified_eui64_to_mac_1
test_modified_eui64_to_mac_1: iid2mac
	@if [ "`./iid2mac ::cc22:4aff:fe08:51a0`" != "ce:22:4a:08:51:a0" ]; \
	then \
	    echo "FAIL: $(@)" 1>&2; \
	else \
	    echo "PASS: $(@)" 1>&2; \
	fi

.PHONY: test_modified_eui64_to_mac_2
test_modified_eui64_to_mac_2: iid2mac
	@if [ "`./iid2mac fe80::cc22:4aff:fe08:51a0`" != "ce:22:4a:08:51:a0" ]; \
	then \
	    echo "FAIL: $(@)" 1>&2; \
	else \
	    echo "PASS: $(@)" 1>&2; \
	fi

mac2iid: mac2iid.o
	@$(CC) $(CFLAGS) -o $@ $^

iid2mac: iid2mac.o
	@$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean
clean:
	@rm -f *.o mac2iid iid2mac
