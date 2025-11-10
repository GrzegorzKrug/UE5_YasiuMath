#pragma once
//#include <stdint.h>
// #include <cstdint>
#include  "CoreMinimal.h"

#include  "SquirrelRNG.generated.h"


UCLASS(BlueprintType)
class YASIUMATH_API USquirrel13_RNG : public UObject {
public:
    GENERATED_BODY()

    USquirrel13_RNG() {};

    USquirrel13_RNG( unsigned int seed )
        : m_seed(seed), init_seed(seed) {};

    USquirrel13_RNG( int position, unsigned int seed )
        : m_position(position), m_seed(seed), init_position(position), init_seed(seed) {};

    USquirrel13_RNG( int position, unsigned int seed, unsigned int variant )
        : m_position(position), m_seed(seed), m_variant(variant), init_position(position), init_seed(seed) {};

protected:
    UPROPERTY()
    int m_position{0};

    UPROPERTY()
    unsigned int m_seed{1};

    UPROPERTY()
    int m_variant{0};

    UPROPERTY()
    unsigned int init_position = 0;

    // UPROPERTY(BlueprintReadWrite)
    int32 init_seed = 0;

public:

    /* Init object */
    void Init(int seed, int position=0);
    
    /* Return uint32_t without changing position */
    uint32_t get_current_random() const; /// Return uint32_t

    /* Move position by 1 and return uint32_t */
    uint32_t get_random();

    /* Move position by 1 and return double in range <0, 1> */
    UFUNCTION(BlueprintCallable)
    int get_random_int(const int& min, const int& max);

    /* Return double in range <0, 1> without changing position */
    UFUNCTION(BlueprintCallable)
    int get_current_int(const int& min, const int& max) const;
    
    /* Move position by 1 and return double in range <0, 1> */
    UFUNCTION(BlueprintCallable)
    double get_random_double();

    /* Return double in range <0, 1> without changing position */
    UFUNCTION(BlueprintCallable)
    double get_current_double() const;

    UFUNCTION(BlueprintCallable)
    void set_position( int new_position );

    UFUNCTION(BlueprintCallable)
    void set_seed( int new_seed );

    UFUNCTION(BlueprintCallable)
    void noiseVariant( int newVariant );


    /* Reset to initial values */
    UFUNCTION(BlueprintCallable)
    void resetSeedPos();

    static uint32_t RNG_0( int position, unsigned int seed );

    static uint32_t RNG_1( int position, unsigned int seed );

    static uint32_t RNG_2( int position, unsigned int seed );

    static uint32_t RNG_3( int position, unsigned int seed );


    void Serialize( FArchive& Ar ) override;
};
