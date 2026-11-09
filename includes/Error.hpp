
#pragma once

// Error throw if the argc < 3
#define ENOE_ARGS "Not enought arguments!"
// Error throw if the argc > 3
#define ETOM_ARGS "Too many arguments!"
// Error throw if a password is too long (1024 char max)
#define EPWD_TOO_LONG "The password is too long!"
// Error throw if the port is not a uint16_t (also known as short)
#define EINVALID_PORT "Invalid port number 1-65535"
// Error throw if a "new" operator failed
#define EMEMORY "Failed to allocated memory!"
