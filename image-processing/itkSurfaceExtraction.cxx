/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#include "itkImageFileReader.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkAntiAliasBinaryImageFilter.h"

int main( int argc, char * argv [] )
{
  const unsigned int Dimension = 3;

  typedef unsigned char                                       InputPixelType;
  typedef itk::Image< InputPixelType, Dimension >             InputImageType;

  typedef itk::ImageFileReader< InputImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }


  typedef itk::Image< unsigned char, Dimension >              BinaryImageType;
  typedef itk::Image< double, Dimension >                     RealImageType;

  typedef itk::BinaryThresholdImageFilter< InputImageType, BinaryImageType >  ThresholdFilterType;

  ThresholdFilterType::Pointer binaryThresholdFilter = ThresholdFilterType::New();

  binaryThresholdFilter->SetInput( reader->GetOutput() );

  typedef itk::AntiAliasBinaryImageFilter< BinaryImageType, RealImageType > AntiAliasFilterType;

  AntiAliasFilterType::Pointer antiAliasFilter = AntiAliasFilterType::New();

  antiAliasFilter->SetNumberOfIterations(50);
  antiAliasFilter->SetInput( binaryThresholdFilter->GetOutput() );

  antiAliasFilter->Update();

  return EXIT_SUCCESS;
}
