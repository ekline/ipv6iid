# ipv6iid
IPv6 Interface Identifier (IID) utility

A short utility to convert MAC addresses to modified EUI64 IPv6 IIDs, and vice versa.

This utility implements the conversion described in [RFC 4291](https://www.rfc-editor.org/rfc/rfc4291.html) Appendix A.

# Examples

```
./ipv6iid ce:22:4a:08:51:a0
::cc22:4aff:fe08:51a0
```

```
./ipv6iid ::cc22:4aff:fe08:51a0
ce:22:4a:8:51:a0
```

```
./ipv6iid fe80::cc22:4aff:fe08:51a0
ce:22:4a:8:51:a0
```
