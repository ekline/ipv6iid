#if 0

MIT License

Copyright (c) 2021 Erik Kline

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

#endif

#include <arpa/inet.h>
#include <assert.h>
#ifdef __APPLE__
#  include <net/ethernet.h>
#else
#  include <netinet/ether.h>
#endif
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>


void usage() {
    fprintf(stderr, "usage: modified_eui64\n\n");
    fprintf(stderr, "Given a modified EUI64 IPv6 IID, return the MAC\n");
}

char* ether2string(const struct ether_addr* mac48) {
    static char buf[] = "11:22:33:44:55:66";
    if (mac48 == NULL) {
        return NULL;
    }

    snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac48->ether_addr_octet[0], mac48->ether_addr_octet[1],
             mac48->ether_addr_octet[2], mac48->ether_addr_octet[3],
             mac48->ether_addr_octet[4], mac48->ether_addr_octet[5]);
    buf[sizeof(buf) - 1] = '\0';
    return buf;
}


int main(int argc, const char* argv[]) {
    assert(6 == sizeof(struct ether_addr));
    assert(16 == sizeof(struct in6_addr));

    if (argc != 2) {
        usage();
        return __LINE__;
    }

    struct in6_addr ipv6;
    memset(&ipv6, 0, sizeof(ipv6));

    // Try to parse the argument as a modified EUI64 IPv6 address.
    if (inet_pton(AF_INET6, argv[1], &ipv6) == 1) {
        if (ipv6.s6_addr[11] != 0xff || ipv6.s6_addr[12] != 0xfe) {
            fprintf(stderr, "%s does not contain a modified EUI64 IPv6 IID\n",
                    argv[1]);
            return __LINE__;
        }

        struct ether_addr mac48;
        memset(&mac48, 0, sizeof(mac48));

        ipv6.s6_addr[8] ^= 0x02;
        for (int i = 0; i < 3; i++) {
            mac48.ether_addr_octet[i] = ipv6.s6_addr[8+i];
            mac48.ether_addr_octet[3+i] = ipv6.s6_addr[13+i];
        }
        fprintf(stdout, "%s\n", ether2string(&mac48));

        return 0;
    }

    // Failed to parse argument.
    usage();
    return __LINE__;
}
