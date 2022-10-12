# FAQ

## Table of Contents

* [Why is the SDK changing so extensively?](#why-is-the-sdk-changing-so-extensively)

<a id="why-is-the-sdk-changing-so-extensively"></a>

## Why is the SDK changing so extensively?

The C (Legacy) API has done an excellent job of supporting the Pixie-16 hardware over many years.
The Legacy API provides a range of general purpose functions aimed at supporting a Visual Basic GUI.
The API functions expose variables and defines reserved for internal use. The static design
complicates the APIs ability to support future hardware updates and changes. All of this comes
together to blur the API boundary and make it harder to ensure a robust product line.

After an extensive software review, it was clear we were not going to be able to maintain full
backwards compatibility with all end-user software. What we can guarantee is that the updated API
will be compatible with Pixie-16 Revision B through F hardware.

By necessity, we will be making changes to the public API calls and the SDK. We will adopt a process
of depreciating interfaces then removing them. We will provide notice of

no less than 12 months before removing an interface, and no less than 6 months before changing an
interface.

Notices will appear in the source code, documentation, and release notes. We will notify users via
email 30 days prior to the deprecation date. Deprecated interfaces will receive bug fixes during
their deprecation phase, but will receive no added functionality. This policy may not cover all
situations, and will evolve over time. For situations that don’t fit neatly into this policy, we
will ensure that the information is available in all relevant Release Notes.

Our focus is to move users towards a consistent, smaller set of interfaces that remain stable as the
products change.

The PixieSDK supports the core functionality needed to operate a crate of modules. This includes
operations like loading firmware, configuring signal processing FPGA parameters, and starting data
runs. We didn’t start completely from scratch and reused much of the solid robust code that’s been
serving you for over a decade. We’ve just moved it all into an extensible C++ framework. C++
provides a portable language with strong concurrency and clock support. A concurrent API is critical
for delivering the best possible performance in a stable and deterministic manner. The PixieSDK
supports

* deterministic access to all hardware resources,
* portable host operating system clocks,
* concurrent API calls,
* and concurrent workers that off-load tasks to provide better system throughput and performance on
  multicore systems.

We’ve wrapped this new functionality into the Pixie16Api.so library. The new API library implements
almost all the functions used in the Legacy API. This allows us to provide consistent logging and
error numbers for a clean user experience. We’re able to expose our threaded list-mode data workers
without the need to update end-user function calls. Most importantly, we’re able to insulate the API
from changes in the hardware. For example, the hardware may expand from 16 to 32 channels without
breaking compatibility with user software.

We fully intend to support the Pixie16Api long term. Future changes to the API will follow our API
update policies described above. We’re actively working on updating our documentation to explain any
changes that you may need to make in order to migrate to the new API.
