// The core-only iOS shell exposes the profiling controls in main.m even when
// the GTA IV native renderer is not built. Keep those UI calls linkable without
// pretending that a capture was started; the full native renderer can provide
// alternate definitions when it is enabled in a non-core build.
extern "C" int rex_gta4_native_profile_start(void) { return 0; }
extern "C" int rex_gta4_native_profile_status(void) { return -1; }
