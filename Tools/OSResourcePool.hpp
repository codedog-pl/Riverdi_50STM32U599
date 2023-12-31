/**
 * @file        OSResourcePool.hpp
 * @author      CodeDog
 *
 * @brief       Defines RTOS resources metadata pools.
 *
 * @remarks     It is used to statically preallocate some handle pools
 *              for the target RTOS, especially one that does not use
 *              dynamic allocation that is to be avoided on embedded.
 *
 * @copyright   (c)2023 CodeDog, All rights reserved.
 */

#include <type_traits>
#include <functional>

using OSResourceId = unsigned int;  // RTOS resource identifier type.

/// @brief RTOS resource metadata structure base class.
struct OSResource
{
    OSResource() : id(0) { }
    OSResourceId id;
};

/// @brief Contains a fixed number of RTOS resource metadata structures.
/// @tparam T Resource type, must inherit from OSResource.
/// @tparam N Number of elements to store.
template<typename T, unsigned int N>
class OSResourcePool
{

public:

    static constexpr unsigned int size = N; // The number of elements available in the pool.

    /// @brief A function that maches a resource.
    using Match = std::function<bool(T&)>;

    OSResourcePool() : m_pool()
    {
        static_assert(std::is_base_of<OSResource, T>::value, "T must inherit from OSResource");
    }

    /// @brief Gets the resource metadata instance by identifier or return the first available.
    /// @param id Resource identifier.
    /// @return Instance pointer or `nullptr` if not available.
    T* getInstance(OSResourceId id = 0)
    {
        if (id > 0 && id <= size && m_pool[id - 1].id == id) return &m_pool[id - 1];
        T* result = nullptr;
        for (unsigned int i = 0; i < size; i++)
        {
            result = &m_pool[i];
            if (!result->id)
            {
                result->id = i + 1;
                return result;
            }
        }
        return nullptr;
    }

    /// @brief Gets the resource metadata instance by using a match function.
    /// @param matchFunction The function taking potential candidate as an argument and returns true if it matches a specific condition.
    /// @return Instance pointer or `nullptr` if not found.
    T* getInstance(Match matchFunction)
    {
        T* result = nullptr;
        for (unsigned int i = 0; i < size; i++)
        {
            result = &m_pool[i];
            if (result->id && matchFunction(*result)) return result;
        }
        return nullptr;
    }

    /// @brief Returns the resource metadata instance to the pool.
    /// @param id Resource indentifier.
    void returnInstance(OSResourceId id)
    {
        if (id > 0 && id <= size && m_pool[id - 1].id == id) m_pool[id - 1].id = 0;
    }

private:
    T m_pool[N]; // Internal pool storage.

};
