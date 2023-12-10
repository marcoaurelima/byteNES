.ORG 0600

LDA #$FF
CMP $33

BNE NotEqual

NotEqual:
LDX #$FE
