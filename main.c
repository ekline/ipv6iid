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
#include <netinet/ether.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>


void usage() {
    fprintf(stderr, "usage: <mac48|modified_eui64>\n\n");
    fprintf(stderr, "Given a MAC, return the modified EUI64 IPv6 IID\n");
    fprintf(stderr, "Given a modified EUI64 IPv6 IID, return the MAC\n");
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

    struct ether_addr *parsed_mac48 = ether_aton(argv[1]);
    if (parsed_mac48 != NULL) {
        for (int i = 0; i < 3; i++) {
            ipv6.s6_addr[8+i] = parsed_mac48->ether_addr_octet[i];
            ipv6.s6_addr[13+i] = parsed_mac48->ether_addr_octet[3+i];
        }
        ipv6.s6_addr[8] ^= 0x02;
        ipv6.s6_addr[11] = 0xff;
        ipv6.s6_addr[12] = 0xfe;

        char buf[INET6_ADDRSTRLEN];
        memset(&buf, 0, sizeof(buf));
        fprintf(stdout, "%s\n", inet_ntop(AF_INET6, &ipv6, buf, sizeof(buf)));

        return 0;
    }

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
        fprintf(stdout, "%s\n", ether_ntoa(&mac48));

        return 0;
    }

    usage();
    return __LINE__;
}
