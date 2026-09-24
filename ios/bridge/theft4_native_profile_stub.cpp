// The core-only iOS shell exposes the profiling controls in main.m even when
// the GTA IV native renderer is not built. Keep those UI calls linkable without
// pretending that a capture was started; the full native renderer can provide
// strong definitions when it is enabled.
extern "C" {
__attribute__((weak)) int rex_gta4_native_profile_start(void) { return 0; }
__attribute__((weak)) int rex_gta4_native_profile_status(void) { return -1; }
}
