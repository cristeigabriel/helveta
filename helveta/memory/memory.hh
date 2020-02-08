/*
*project name: helveta
*purpose: multi-purpose shared library for windows, made to be submodule for projects
*written by: Cristei Gabriel-Marian [ + thanks to Laxol for advising me. ]
*licensing: MIT License

*file description: internal and external memory handler
*/

#include <Windows.h>

namespace util {

    namespace memory {

        // windows api read wrapper
        const HANDLE read( const HANDLE& process_handle,
                           const std::uintptr_t read_location,
                           const std::uintptr_t& read_size ) {

            void* read_value;

            if ( !ReadProcessMemory( process_handle, ( HANDLE )read_location, &read_value,
                 read_size, nullptr ) ) {

                static_assert( "[HELVETA] Failed to read memory." );
                return nullptr;
            }

            return read_value;
        }

        // windows api write wrapper
        bool write( const HANDLE& process_handle, const std::uintptr_t write_location,
                    const void*& data_to_write,
                    const std::size_t& data_size ) {

            bool write_successful = WriteProcessMemory(
                process_handle, ( HANDLE )write_location, data_to_write, data_size, nullptr );

            return write_successful;
        }
    } // namespace memory
} // namespace util