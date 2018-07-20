#pragma once

#include "AbstractMIDIOutput.hpp"
#include <Hardware/ButtonMatrix.h>
#include <Helpers/Array.hpp>

template <uint8_t nb_rows, uint8_t nb_cols>
using AddressMatrix = Array<Array<uint8_t, nb_cols>, nb_rows>;

/**
 * @brief   MIDIButtonMatrix
 * @todo    Documentation.
 * @see     ButtonMatrix
 */
template <class Sender, uint8_t nb_rows, uint8_t nb_cols>
class MIDIButtonMatrix : public AbstractMIDIOutput,
                         public ButtonMatrix<nb_rows, nb_cols> {

  public:
    /**
     * @brief
     * @todo    Documentation
     */
    MIDIButtonMatrix(const pin_t (&rowPins)[nb_rows],
                     const pin_t (&colPins)[nb_cols],
                     const AddressMatrix<nb_rows, nb_cols> &addresses,
                     uint8_t channel)
        : ButtonMatrix<nb_rows, nb_cols>(rowPins, colPins),
          addresses(addresses), baseChannel(channel) {}

    void update() final override { ButtonMatrix<nb_rows, nb_cols>::refresh(); }

  private:
    void onButtonChanged(uint8_t row, uint8_t col, bool state) final override {
        uint8_t address = addresses[row][col];
        if (state)
            Sender::sendOff(baseChannel, address);
        else
            Sender::sendOn(baseChannel, address);
    }

    AddressMatrix<nb_rows, nb_cols> addresses;
    const uint8_t baseChannel;
};