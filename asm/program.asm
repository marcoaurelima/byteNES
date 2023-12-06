JSR init
JSR loop
JSR end

init:
  LDX #$11
  RTS

loop:
  INX
  CPX #$05
  RTS

end:
  INX
