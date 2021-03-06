//
// Copyright © 2017 Arm Ltd. All rights reserved.
// See LICENSE file in the project root for full license information.
//
#include <boost/test/unit_test.hpp>
#include "armnnTfLiteParser/ITfLiteParser.hpp"
#include "ParserFlatbuffersFixture.hpp"

BOOST_AUTO_TEST_SUITE(TensorflowLiteParser)

struct AvgPool2DFixture : public ParserFlatbuffersFixture
{
    explicit AvgPool2DFixture(std::string inputdim, std::string outputdim, std::string dataType)
    {
        m_JsonString = R"(
        {
            "version": 3,
            "operator_codes": [ { "builtin_code": "AVERAGE_POOL_2D" } ],
            "subgraphs": [
            {
                "tensors": [
                {
                    "shape": )"
                    + outputdim
                    + R"(,
                    "type": )"
                      + dataType
                      + R"(,
                            "buffer": 0,
                            "name": "OutputTensor",
                            "quantization": {
                                "min": [ 0.0 ],
                                "max": [ 255.0 ],
                                "scale": [ 1.0 ],
                                "zero_point": [ 0 ]
                            }
                },
                {
                    "shape": )"
                    + inputdim
                    + R"(,
                    "type": )"
                      + dataType
                      + R"(,
                            "buffer": 1,
                            "name": "InputTensor",
                            "quantization": {
                                "min": [ 0.0 ],
                                "max": [ 255.0 ],
                                "scale": [ 1.0 ],
                                "zero_point": [ 0 ]
                            }
                }
                ],
                "inputs": [ 1 ],
                "outputs": [ 0 ],
                "operators": [ {
                        "opcode_index": 0,
                        "inputs": [ 1 ],
                        "outputs": [ 0 ],
                        "builtin_options_type": "Pool2DOptions",
                        "builtin_options":
                        {
                            "padding": "VALID",
                            "stride_w": 2,
                            "stride_h": 2,
                            "filter_width": 2,
                            "filter_height": 2,
                            "fused_activation_function": "NONE"
                        },
                        "custom_options_format": "FLEXBUFFERS"
                    } ]
                }
            ],
            "description": "AvgPool2D test.",
            "buffers" : [ {}, {} ]
        })";

        SetupSingleInputSingleOutput("InputTensor", "OutputTensor");
    }
};


struct AvgPoolLiteFixtureUint1DOutput : AvgPool2DFixture
{
    AvgPoolLiteFixtureUint1DOutput() : AvgPool2DFixture("[ 1, 2, 2, 1 ]", "[ 1, 1, 1, 1 ]", "UINT8") {}
};

struct AvgPoolLiteFixtureFloat1DOutput : AvgPool2DFixture
{
    AvgPoolLiteFixtureFloat1DOutput() : AvgPool2DFixture("[ 1, 2, 2, 1 ]", "[ 1, 1, 1, 1 ]", "FLOAT32") {}
};

struct AvgPoolLiteFixture2DOutput : AvgPool2DFixture
{
    AvgPoolLiteFixture2DOutput() : AvgPool2DFixture("[ 1, 4, 4, 1 ]", "[ 1, 2, 2, 1 ]", "UINT8") {}
};

BOOST_FIXTURE_TEST_CASE(AvgPoolLite1DOutput, AvgPoolLiteFixtureUint1DOutput)
{
    RunTest<4, uint8_t>(0, {2, 3, 5, 2 }, { 3 });
}

BOOST_FIXTURE_TEST_CASE(AvgPoolLiteFloat1DOutput, AvgPoolLiteFixtureFloat1DOutput)
{
    RunTest<4, float>(0, { 2.0f, 3.0f, 5.0f, 2.0f },  { 3.0f });
}

BOOST_FIXTURE_TEST_CASE(AvgPoolLite2DOutput, AvgPoolLiteFixture2DOutput)
{
    RunTest<4, uint8_t>(0, { 1, 2, 2, 3, 5, 6, 7, 8, 3, 2, 1, 0, 1, 2, 3, 4 }, { 4, 5, 2, 2 });
}

BOOST_FIXTURE_TEST_CASE(IncorrectDataTypeError, AvgPoolLiteFixtureFloat1DOutput)
{
    BOOST_CHECK_THROW((RunTest<4, uint8_t>(0, {2, 3, 5, 2 }, { 3 })), armnn::Exception);
}

BOOST_AUTO_TEST_SUITE_END()
