# JUCE Plugin Architecture

**Structure for DSP testbed plugins.**

---

## AudioProcessor Class

Main processing class inherits from `juce::AudioProcessor`:

```cpp
class MyDSPProcessor : public juce::AudioProcessor {
public:
    MyDSPProcessor();
    ~MyDSPProcessor() override;

    // Audio processing
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) override;
    void releaseResources() override;

    // Plugin lifecycle
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;

    // State management
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

private:
    // DSP state (e.g., biquad filters, buffers)
    juce::dsp::ProcessorChain<...> processorChain;
};
```

---

## processBlock() Method

Called by host at audio rate:

```cpp
void MyDSPProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Process audio
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        auto* channelData = buffer.getWritePointer(channel);
        // Your DSP here
    }
}
```

---

## Parameter Management

Use APVTS (AudioProcessorValueTreeState) for parameters:

```cpp
juce::AudioProcessorValueTreeState parameters;

// Constructor
MyDSPProcessor::MyDSPProcessor()
    : parameters(*this, nullptr, "Parameters", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "gain", "Gain", 0.0f, 1.0f, 0.5f));

    return { params.begin(), params.end() };
}
```

---

## Integration with Embedded Code

JUCE testbed should mirror embedded ProcessBlock interface:

```cpp
void ProcessBlock(float* input, float* output, int numSamples) {
    // Embedded DSP logic
}

// JUCE wrapper
void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    auto* channelData = buffer.getWritePointer(0);
    ProcessBlock(channelData, channelData, buffer.getNumSamples());
}
```

---

*Load this file for JUCE plugin structure.*
