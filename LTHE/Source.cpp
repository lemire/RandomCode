#include <stdio.h>
#include "LTHE.h"
#include <chrono>

//=================================================================================
// times a block of code
struct SBlockTimer
{
    SBlockTimer()
    {
        m_start = std::chrono::high_resolution_clock::now();
    }

    ~SBlockTimer()
    {
        std::chrono::duration<float> seconds = std::chrono::high_resolution_clock::now() - m_start;
        printf("    %0.2f second\n", seconds.count());
    }

    std::chrono::high_resolution_clock::time_point m_start;
};

//=================================================================================
float TransformData (float value)
{
    return (float)sqrt(value * 2.17f + 0.132);
}

//=================================================================================
int main (int argc, char **argv)
{
    /*
    // Encrypt the data
    printf("Encrypting data...\n");
    std::vector<float> secretValues = { 3.14159265359f, 435.0f };
    std::vector<size_t> keys;
    {
        SBlockTimer timer;
        if (!LTHE::Encrypt(secretValues, 100000000, "Encrypted.dat", keys))
        {
            fprintf(stderr, "Could not encrypt data.\n");
            return -1;
        }
    }

    // Transform the data
    printf("Transforming data...\n");
    {
        SBlockTimer timer;
        if (!LTHE::TransformHomomorphically<float>("Encrypted.dat", TransformData))
        {
            fprintf(stderr, "Could not transform encrypt data.\n");
            return -2;
        }
    }

    // Decrypt the data
    printf("Decrypting data...\n");
    std::vector<float> decryptedValues;
    {
        SBlockTimer timer;
        if (!LTHE::Decrypt("Encrypted.dat", decryptedValues, keys))
        {
            fprintf(stderr, "Could not decrypt data.\n");
            return -3;
        }
    }

    // Verify the data
    printf("Verifying data...\n");
    {
        SBlockTimer timer;
        for (size_t i = 0, c = secretValues.size(); i < c; ++i)
        {
            if (TransformData(secretValues[i]) != decryptedValues[i])
            {
                fprintf(stderr, "decrypted value mismatch!\n");
                return -4;
            }
        }
    }
    */

    struct SBlah
    {
        uint8_t x, y, z;

        static SBlah Transform (SBlah b)
        {
            return SBlah{ uint8_t(b.x * 2), uint8_t(b.y * 3), uint8_t(b.z * 4) };
        }

        bool operator != (const SBlah& b) const
        {
            return b.x != x || b.y != y || b.z != z;
        }
    };

    // Encrypt the data
    printf("Encrypting data...\n");
    std::vector<SBlah> secretValues = { {0,1,2},{ 3,4,5 },{ 6,7,8 } };
    std::vector<size_t> keys;
    {
        SBlockTimer timer;
        if (!LTHE::Encrypt(secretValues, 1000002, "Encrypted.dat", keys))
        {
            fprintf(stderr, "Could not encrypt data.\n");
            return -1;
        }
    }

    // Transform the data
    printf("Transforming data...\n");
    {
        SBlockTimer timer;
        if (!LTHE::TransformHomomorphically<SBlah>("Encrypted.dat", SBlah::Transform))
        {
            fprintf(stderr, "Could not transform encrypt data.\n");
            return -2;
        }
    }

    // Decrypt the data
    printf("Decrypting data...\n");
    std::vector<SBlah> decryptedValues;
    {
        SBlockTimer timer;
        if (!LTHE::Decrypt("Encrypted.dat", decryptedValues, keys))
        {
            fprintf(stderr, "Could not decrypt data.\n");
            return -3;
        }
    }

    // Verify the data
    printf("Verifying data...\n");
    {
        SBlockTimer timer;
        for (size_t i = 0, c = secretValues.size(); i < c; ++i)
        {
            if (SBlah::Transform(secretValues[i]) != decryptedValues[i])
            {
                fprintf(stderr, "decrypted value mismatch!\n");
                return -4;
            }
        }
    }

    printf("Finished, everything checked out!\n");
    return 0;
}

/*

TODO:
* 3 byte struct has issues, not surviving round trip! need to see why
* make sure the file is the exact right size each time

Blog:
* Can encrypt M items by having the be in a list of N items
* Send that list of values to someone else
* They do operations on every item on the list and send it back

* further details
 * the N items should not make your data stand out. If your numbers are floats, don't generate a bunch of integers.
 * the larger the N, the more secure.
 * up to having the full space of values possible represented.
 * show how big that is in bytes for some value types.
 * when doing that, you don't even need to send the values to the person doing the computation, they can just send the results back.
  * could maybe even send back some of the data (like 2/3) and you could use the data if there, else call it a failed calculation and handle the failure gracefully, without asking again, to not give a hint if you got it or not.
 * The larger the M, the less secure
 * report some timing on your blog.

* For faster processing:
 * SIMD / multithreaded.
 * GPU
 ? can we benchmark?
 ! would be most robust to compare memory usage and execution speed to other FHE but i dont have time for that :p

--Links--
Fully Homomorphic SIMD Operations
http://homes.esat.kuleuven.be/~fvercaut/papers/DCC2011.pdf

*/