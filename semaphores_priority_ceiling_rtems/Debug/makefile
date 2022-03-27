################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: semaphores_priority_ceiling_rtems

# Tool invocations
semaphores_priority_ceiling_rtems: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: SPARC RTEMS C Linker'
	sparc-rtems-gcc -msoft-float -o "semaphores_priority_ceiling_rtems" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) semaphores_priority_ceiling_rtems
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
