################################################################################
# IO constraints
################################################################################
# serial:0.tx
set_property LOC D4 [get_ports {serial_tx}]
set_property IOSTANDARD LVCMOS33 [get_ports {serial_tx}]

# serial:0.rx
set_property LOC C4 [get_ports {serial_rx}]
set_property IOSTANDARD LVCMOS33 [get_ports {serial_rx}]

# clk100:0
set_property LOC E3 [get_ports {clk100}]
set_property IOSTANDARD LVCMOS33 [get_ports {clk100}]

# cpu_reset:0
set_property LOC C12 [get_ports {cpu_reset}]
set_property IOSTANDARD LVCMOS33 [get_ports {cpu_reset}]

# user_led:0
set_property LOC T8 [get_ports {user_led0}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led0}]

# user_led:1
set_property LOC V9 [get_ports {user_led1}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led1}]

# user_led:2
set_property LOC R8 [get_ports {user_led2}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led2}]

# user_led:3
set_property LOC T6 [get_ports {user_led3}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led3}]

# user_led:4
set_property LOC T5 [get_ports {user_led4}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led4}]

# user_led:5
set_property LOC T4 [get_ports {user_led5}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led5}]

# user_led:6
set_property LOC U7 [get_ports {user_led6}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led6}]

# user_led:7
set_property LOC U6 [get_ports {user_led7}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led7}]

# user_led:8
set_property LOC V4 [get_ports {user_led8}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led8}]

# user_led:9
set_property LOC U3 [get_ports {user_led9}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led9}]

# user_led:10
set_property LOC V1 [get_ports {user_led10}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led10}]

# user_led:11
set_property LOC R1 [get_ports {user_led11}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led11}]

# user_led:12
set_property LOC P5 [get_ports {user_led12}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led12}]

# user_led:13
set_property LOC U1 [get_ports {user_led13}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led13}]

# user_led:14
set_property LOC R2 [get_ports {user_led14}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led14}]

# user_led:15
set_property LOC P2 [get_ports {user_led15}]
set_property IOSTANDARD LVCMOS33 [get_ports {user_led15}]

# uart1:0.tx
set_property LOC H4 [get_ports {uart1_tx}]
set_property IOSTANDARD LVCMOS33 [get_ports {uart1_tx}]

# uart1:0.rx
set_property LOC H1 [get_ports {uart1_rx}]
set_property IOSTANDARD LVCMOS33 [get_ports {uart1_rx}]

################################################################################
# Design constraints
################################################################################

set_property INTERNAL_VREF 0.750 [get_iobanks 34]

################################################################################
# Clock constraints
################################################################################


create_clock -name clk100 -period 10.0 [get_nets clk100]

################################################################################
# False path constraints
################################################################################


set_false_path -quiet -through [get_nets -hierarchical -filter {mr_ff == TRUE}]

set_false_path -quiet -to [get_pins -filter {REF_PIN_NAME == PRE} -of_objects [get_cells -hierarchical -filter {ars_ff1 == TRUE || ars_ff2 == TRUE}]]

set_max_delay 2 -quiet -from [get_pins -filter {REF_PIN_NAME == C} -of_objects [get_cells -hierarchical -filter {ars_ff1 == TRUE}]] -to [get_pins -filter {REF_PIN_NAME == D} -of_objects [get_cells -hierarchical -filter {ars_ff2 == TRUE}]]