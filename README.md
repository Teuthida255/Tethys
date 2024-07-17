# Tethys

**Tethys** is the name of an ongoing series of Sega Saturn audio experiments by [Teuthida](https://github.com/Teuthida255) designed to explore the potential of the console's sound chip, the Sega Custom Sound Processor (_SCSP_). At present, Tethys consists of a single program:
- **Calypso**, an interactive instrument editor that showcases the SCSP's core capabilities, with support for custom instrument envelopes ('Macros') and samples.

Tethys's features are built off of the extensive foundation provided by the [ponèSound](https://github.com/ponut64/SCSP_poneSound) sound driver developed by [ponut64](https://github.com/ponut64), with modifications to support more of the SCSP's features and a new code interface geared towards sequenced melodic instruments instead of sound effects or prerecorded music. This has also resulted in the (hopefully temporary) removal of ADX audio support from the driver due to performance conflicts.

## Downloads and Usage

The disc image files and build tools for each program can be found in their respective folders (currently just 'Calypso'). You will need the [Jo Engine](https://jo-engine.org/) to compile the programs from the source code, as Tethys relies on the engine for several important system and graphics functions; information about adding the engine and compiling the images can be found in the text files included within each individual program's folder.

[Mednafen's](https://mednafen.github.io/) strong audio emulation makes it the emulator of choice for running Tethy's programs on a computer, and was the primary testing target during development. As such, *running these programs on other emulators or real hardware may result in unintended bugs or audio errors.* Mednafen is a command line-only program, although you can use the [Mednaffe](https://github.com/AmatCoder/mednaffe) frontend if you prefer a GUI-based interface.