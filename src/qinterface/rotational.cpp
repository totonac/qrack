//////////////////////////////////////////////////////////////////////////////////////
//
// (C) Daniel Strano and the Qrack contributors 2017, 2018. All rights reserved.
//
// This is a multithreaded, universal quantum register simulation, allowing
// (nonphysical) register cloning and direct measurement of probability and
// phase, to leverage what advantages classical emulation of qubits can have.
//
// Licensed under the GNU General Public License V3.
// See LICENSE.md in the project root or https://www.gnu.org/licenses/gpl-3.0.en.html
// for details.

#include "qinterface.hpp"

namespace Qrack {

/// "Phase shift gate" - Rotates as e^(-i*\theta/2) around |1> state
void QInterface::RT(real1 radians, bitLenInt qubit)
{
    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    const complex mtrx[4] = { complex(ONE_R1, 0), complex(ZERO_R1, ZERO_R1), complex(ZERO_R1, ZERO_R1),
        complex(cosine, sine) };
    ApplySingleBit(mtrx, true, qubit);
}

/// x axis rotation gate - Rotates as e^(-i*\theta/2) around Pauli x axis
void QInterface::RX(real1 radians, bitLenInt qubit)
{
    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    complex pauliRX[4] = { complex(cosine, ZERO_R1), complex(ZERO_R1, -sine), complex(ZERO_R1, -sine),
        complex(cosine, ZERO_R1) };
    ApplySingleBit(pauliRX, true, qubit);
}

/// y axis rotation gate - Rotates as e^(-i*\theta/2) around Pauli y axis
void QInterface::RY(real1 radians, bitLenInt qubit)
{
    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    complex pauliRY[4] = { complex(cosine, ZERO_R1), complex(-sine, ZERO_R1), complex(sine, ZERO_R1),
        complex(cosine, ZERO_R1) };
    ApplySingleBit(pauliRY, true, qubit);
}

/// z axis rotation gate - Rotates as e^(-i*\theta/2) around Pauli z axis
void QInterface::RZ(real1 radians, bitLenInt qubit)
{
    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    const complex pauliRZ[4] = { complex(cosine, -sine), complex(ZERO_R1, ZERO_R1), complex(ZERO_R1, ZERO_R1),
        complex(cosine, sine) };
    ApplySingleBit(pauliRZ, true, qubit);
}

/// Exponentiate identity operator
void QInterface::Exp(real1 radians, bitLenInt qubit)
{
    complex phaseFac = complex(cos(radians), sin(radians));
    complex expIdentity[4] = { phaseFac, complex(ZERO_R1, ZERO_R1), complex(ZERO_R1, ZERO_R1), phaseFac };
    ApplySingleBit(expIdentity, true, qubit);
}

/// Exponentiate Pauli X operator
void QInterface::ExpX(real1 radians, bitLenInt qubit)
{
    complex phaseFac = complex(cos(radians), sin(radians));
    complex expPauliX[4] = { complex(ZERO_R1, ZERO_R1), phaseFac, phaseFac, complex(ZERO_R1, ZERO_R1) };
    ApplySingleBit(expPauliX, true, qubit);
}

/// Exponentiate Pauli Y operator
void QInterface::ExpY(real1 radians, bitLenInt qubit)
{
    complex phaseFac = complex(cos(radians), sin(radians));
    complex expPauliY[4] = { complex(ZERO_R1, ZERO_R1), phaseFac * complex(ZERO_R1, -ONE_R1),
        phaseFac * complex(ZERO_R1, ONE_R1), complex(ZERO_R1, ZERO_R1) };
    ApplySingleBit(expPauliY, true, qubit);
}

/// Exponentiate Pauli Z operator
void QInterface::ExpZ(real1 radians, bitLenInt qubit)
{
    complex phaseFac = complex(cos(radians), sin(radians));
    complex expPauliZ[4] = { phaseFac, complex(ZERO_R1, ZERO_R1), complex(ZERO_R1, ZERO_R1), -phaseFac };
    ApplySingleBit(expPauliZ, true, qubit);
}

/// Controlled "phase shift gate" - if control bit is true, rotates target bit as e^(-i*\theta/2) around |1> state
void QInterface::CRT(real1 radians, bitLenInt control, bitLenInt target)
{
    if (control == target) {
        throw std::invalid_argument("control bit cannot also be target.");
    }

    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    const complex mtrx[4] = { complex(ONE_R1, 0), complex(ZERO_R1, ZERO_R1), complex(ZERO_R1, ZERO_R1),
        complex(cosine, sine) };
    bitLenInt controls[1] = { control };
    ApplyControlledSingleBit(controls, 1, target, mtrx);
}

/// Controlled x axis rotation - if control bit is true, rotates as e^(-i*\theta/2) around Pauli x axis
void QInterface::CRX(real1 radians, bitLenInt control, bitLenInt target)
{
    if (control == target)
        throw std::invalid_argument("CRX control bit cannot also be target.");
    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    complex pauliRX[4] = { complex(cosine, ZERO_R1), complex(ZERO_R1, -sine), complex(ZERO_R1, -sine),
        complex(cosine, ZERO_R1) };
    bitLenInt controls[1] = { control };
    ApplyControlledSingleBit(controls, 1, target, pauliRX);
}

/// Controlled y axis rotation - if control bit is true, rotates as e^(-i*\theta) around Pauli y axis
void QInterface::CRY(real1 radians, bitLenInt control, bitLenInt target)
{
    if (control == target)
        throw std::invalid_argument("CRY control bit cannot also be target.");
    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    complex pauliRY[4] = { complex(cosine, ZERO_R1), complex(-sine, ZERO_R1), complex(sine, ZERO_R1),
        complex(cosine, ZERO_R1) };
    bitLenInt controls[1] = { control };
    ApplyControlledSingleBit(controls, 1, target, pauliRY);
}

/// Controlled z axis rotation - if control bit is true, rotates as e^(-i*\theta) around Pauli z axis
void QInterface::CRZ(real1 radians, bitLenInt control, bitLenInt target)
{
    if (control == target)
        throw std::invalid_argument("CRZ control bit cannot also be target.");
    real1 cosine = cos(radians / 2.0);
    real1 sine = sin(radians / 2.0);
    const complex pauliRZ[4] = { complex(cosine, -sine), complex(ZERO_R1, ZERO_R1), complex(ZERO_R1, ZERO_R1),
        complex(cosine, sine) };
    bitLenInt controls[1] = { control };
    ApplyControlledSingleBit(controls, 1, target, pauliRZ);
}

} // namespace Qrack
