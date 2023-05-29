#ifndef __FFT_H__
#define __FFT_H__

#include "mem.h"
#include <memory>
#include <cmath>
#include "log2.h"
#include "limits.h"
#include <iostream>
#include "BoolVector.h"
#include "Serializer.h"

namespace fft{

    static const double PI = 3.14159265358979323846;

    struct ComplexNumber{

        double im;
        double real;
        
    };

    class LinearProjectable{

        public:

            virtual void to_im_plane(double *, ComplexNumber *, size_t) = 0;
            virtual void to_real_plane(ComplexNumber *, double *, size_t) = 0;

    };

    class ComplexNumberMultipliable{

        public:

            virtual ComplexNumber multiply(ComplexNumber, ComplexNumber) = 0; 
            
    };
    
    class ComplexNumberDividable{

        public:

            virtual ComplexNumber divide(ComplexNumber, size_t) = 0;
        
    };

    class ComplexNumberPlusable{

        public:
            
            virtual ComplexNumber plus(ComplexNumber, ComplexNumber) = 0;

    };

    class FFTMultiplierRetrievable{

        public:

            virtual ComplexNumber get(size_t, size_t) = 0; 

    };

    class FFTPrecalculatedMultiplierRetrievable: public virtual FFTMultiplierRetrievable{

        public:
            
            virtual size_t get_instantiation_cost(size_t) = 0;
            virtual void prep(size_t, void *) = 0;

    }; 

    class Conjugatable{

        public:

            virtual ComplexNumber conjugate(ComplexNumber) = 0;

    };

    class InPlaceTransformable{

        public:

            virtual void transform(ComplexNumber *, size_t) = 0;
            virtual void inverse_transform(ComplexNumber *, size_t) = 0;


    };

    class InPlaceTransformGeneratable{

        public:

            virtual std::shared_ptr<InPlaceTransformable> get(memory::linear::TempStorageGeneratable *) = 0;

    };

    class LinearConvolutable{

        public:

            virtual void convolute(double *, double *, size_t, double *) = 0;

    };

    class LinearConvolutorGeneratable{

        public:

            virtual std::shared_ptr<LinearConvolutable> get(memory::linear::TempStorageGeneratable *) = 0;

    };

    template <class T>
    void linear_transform(ComplexNumber * arr, size_t n, T transformer){

        for (size_t i = 0; i < n; ++i){

            arr[i] = transformer(arr[i]);

        }

    };

}

namespace fft{

    class LinearProjector: public virtual LinearProjectable{

        public:

            void to_im_plane(double * data, ComplexNumber * converted_data, size_t length){

                ComplexNumber cur;
                cur.im = 0;

                for (size_t i = 0; i < length; ++i){

                    cur.real = data[i];
                    converted_data[i] = cur;

                }

            }

            void to_real_plane(ComplexNumber * data, double * converted_data, size_t length){

                for (size_t i = 0; i < length; ++i){

                    converted_data[i] = data[i].real;

                }

            }

    };

    class StandardComplexNumberMultiplier: public virtual ComplexNumberMultipliable{

        public:

            ComplexNumber multiply(ComplexNumber lhs, ComplexNumber rhs){

                ComplexNumber rs;

                rs.real = lhs.real * rhs.real - lhs.im * rhs.im;
                rs.im = lhs.real * rhs.im + lhs.im * rhs.real;

                return rs;

            }

    };

    class StandardComplexNumberDivider: public virtual ComplexNumberDividable{

        public:

            ComplexNumber divide(ComplexNumber lhs, size_t rhs){

                lhs.im /= rhs;
                lhs.real /= rhs;

                return lhs;

            }

    };
    
    class StandardComplexNumberPluser: public virtual ComplexNumberPlusable{

        public:

            ComplexNumber plus(ComplexNumber lhs, ComplexNumber rhs){

                lhs.real += rhs.real;
                lhs.im += rhs.im;

                return lhs;

            }

            void inplace_plus(ComplexNumber& lhs, ComplexNumber& rhs){

                lhs.real += rhs.real;
                lhs.im += rhs.im;

            }

    };

    class StandardConjugator: public virtual Conjugatable{

        public:

            ComplexNumber conjugate(ComplexNumber val){

                val.im = -val.im;

                return val;

            }

    };

    class StandardFFTMultiplierRetriever: public virtual FFTMultiplierRetrievable{

        public:

            ComplexNumber get(size_t idx, size_t length){

                ComplexNumber rs;

                //e^(-j*2pi*k/N) = cos(-2pik/N) + i * sin(-2pik/N) 

                rs.real = std::cos(-2. * PI * idx / length);
                rs.im = std::sin(-2. * PI * idx / length);

                return rs; 

            }

    };

    class StandardFFTPrecalculatedMultiplierArchive{

        private:

            inline static std::pair<size_t, ComplexNumber *> archived_data = std::make_pair(0, nullptr); 

        public:

            std::pair<size_t, ComplexNumber *> get_archive(){
                
                return this->archived_data;

            }

            void set_max_archive(size_t n, ComplexNumber * data, size_t data_sz){

                if (n <= this->archived_data.first){

                    return;

                }
                                
                std::free(this->archived_data.second);

                ComplexNumber * copied = (ComplexNumber *) std::malloc(data_sz * sizeof(ComplexNumber));
                std::memcpy(copied, data, data_sz * sizeof(ComplexNumber));
                
                this->archived_data = std::make_pair(n, copied);

            };


    };

    class StandardFFTPrecalculatedMultiplierRetriever: public StandardFFTMultiplierRetriever, public virtual FFTPrecalculatedMultiplierRetrievable{

        protected:

            ComplexNumber * data;
            size_t n;

        public:

            StandardFFTPrecalculatedMultiplierRetriever(){

                this->data = nullptr;
                this->n = 0;

            }
            
            size_t get_instantiation_cost(size_t n){

                return sizeof(ComplexNumber) * (n << 1); 

            }

            void prep(size_t n, void * plm_space){

                assert(n != 0);
                assert((n & (n - 1)) == 0);

                this->n = n;

                size_t virtual_n = 1;
                size_t bit_length = algorithm::log2::log2_64(n) + 1;
                size_t counter = 0;

                this->data = (ComplexNumber *) plm_space;

                for (size_t i = 0; i < bit_length; ++i){
                    
                    for (size_t j = 0; j < virtual_n; ++j){

                        data[counter++] = StandardFFTMultiplierRetriever::get(j, virtual_n);

                    }

                    virtual_n <<= 1;
                                        
                }

            }

            ComplexNumber get(size_t idx, size_t length){
                
                return this->data[length - 1 + idx];

            }

            ComplexNumber& get_rvalue(size_t idx, size_t length){

                return this->data[length - 1 + idx]; 

            }

    };

    class StandardArchivedFFTPrecalculatedMultiplierRetriever: public StandardFFTMultiplierRetriever, public virtual FFTPrecalculatedMultiplierRetrievable{

        protected:

            ComplexNumber * data;

        public:

            StandardArchivedFFTPrecalculatedMultiplierRetriever(){

                this->data = nullptr;

            }
            
            size_t get_instantiation_cost(size_t n){

                if (StandardFFTPrecalculatedMultiplierArchive().get_archive().first >= n){
                    
                    return 1;

                }

                return sizeof(ComplexNumber) * (n << 1); 

            }

            void prep(size_t n, void * plm_space){

                assert(n != 0);
                assert((n & (n - 1)) == 0);

                if (StandardFFTPrecalculatedMultiplierArchive().get_archive().first < n){
                    
                    size_t virtual_n = 1;
                    size_t bit_length = algorithm::log2::log2_64(n) + 1;
                    size_t counter = 1;
                    
                    this->data = (ComplexNumber *) plm_space;

                    for (size_t i = 0; i < bit_length; ++i){
                        
                        for (size_t j = 0; j < virtual_n; ++j){

                            this->data[counter++] = StandardFFTMultiplierRetriever::get(j, virtual_n);

                        }

                        virtual_n <<= 1;
                                            
                    }

                    StandardFFTPrecalculatedMultiplierArchive().set_max_archive(n, this->data, counter);

                }

                this->data = StandardFFTPrecalculatedMultiplierArchive().get_archive().second;
  
            }

            ComplexNumber get(size_t idx, size_t length){
                
                return this->data[length + idx];

            }

            ComplexNumber& get_rvalue(size_t idx, size_t length){

                return this->data[length + idx]; 

            }

    };

    class TunedFFTPrecalculatedMultiplierRetriever: public StandardFFTPrecalculatedMultiplierRetriever{

        protected:

            const size_t MAX_N = 1 << 20;

        public:

            TunedFFTPrecalculatedMultiplierRetriever(): StandardFFTPrecalculatedMultiplierRetriever(){};

            size_t get_instantiation_cost(size_t n){

                return StandardFFTPrecalculatedMultiplierRetriever::get_instantiation_cost(std::min(MAX_N, n));

            }

            void prep(size_t n, void * plm_space){
                
                StandardFFTPrecalculatedMultiplierRetriever::prep(std::min(MAX_N, n), plm_space);

            }

            ComplexNumber get(size_t idx, size_t length){

                if (length <= MAX_N){

                    return StandardFFTPrecalculatedMultiplierRetriever::get(idx, length);
                
                }

                return StandardFFTMultiplierRetriever::get(idx, length);

            }

    };

    class DFT: public InPlaceTransformable{

        private:

            std::shared_ptr<ComplexNumberPlusable> plus_ops;
            std::shared_ptr<ComplexNumberMultipliable> multiply_ops;
            std::shared_ptr<FFTMultiplierRetrievable> multiplier_retriever;
            std::shared_ptr<ComplexNumberDividable> divide_ops;
            std::shared_ptr<Conjugatable> conjugator;
            
            memory::linear::TempStorageGeneratable * temp_storage;
        
        public:

            DFT(std::shared_ptr<ComplexNumberPlusable> plus_ops,
                std::shared_ptr<ComplexNumberMultipliable> multiply_ops,
                std::shared_ptr<FFTMultiplierRetrievable> multiplier_retriever,
                std::shared_ptr<ComplexNumberDividable> divide_ops,
                std::shared_ptr<Conjugatable> conjugator,

                memory::linear::TempStorageGeneratable * temp_storage){
                

                this->plus_ops = plus_ops;
                this->multiply_ops = multiply_ops;
                this->multiplier_retriever = multiplier_retriever;
                this->divide_ops = divide_ops;
                this->conjugator = conjugator;
                this->temp_storage = temp_storage;

            }

            void transform(ComplexNumber * data, size_t length){
                
                assert(length > 0);
                    
                this->temp_storage->enter_scope();

                ComplexNumber * temp_data = (ComplexNumber *) this->temp_storage->get(length * sizeof(ComplexNumber));

                for (size_t i = 0; i < length; ++i){

                    temp_data[i] = this->dft(data, length, i);

                }

                for (size_t i = 0; i < length; ++i){

                    data[i] = temp_data[i];

                }

                this->temp_storage->exit_scope();

            }

            void inverse_transform(ComplexNumber * data, size_t length){

                auto cjg_lambda = [ops = this->conjugator](ComplexNumber e){return ops->conjugate(e);};
                auto divide_lambda = [ops = this->divide_ops, length](ComplexNumber e){return ops->divide(e, length);}; 

                linear_transform(data, length, cjg_lambda);
                this->transform(data, length);
                linear_transform(data, length, cjg_lambda);
                linear_transform(data, length, divide_lambda);

            }   

        
        private:

            ComplexNumber dft(ComplexNumber * data, size_t n, size_t j){

                ComplexNumber rs;

                rs.real = 0;
                rs.im = 0;

                for (size_t i = 0; i < n; ++i){

                    rs = this->plus_ops->plus(rs, this->multiply_ops->multiply(data[i], this->multiplier_retriever->get(i * j, n)));

                }

                return rs; 
                
            }


    };

    class FastInPlaceTransform: public InPlaceTransformable{

        private:

            std::shared_ptr<ComplexNumberPlusable> plus_ops;
            std::shared_ptr<ComplexNumberMultipliable> multiply_ops;
            std::shared_ptr<FFTPrecalculatedMultiplierRetrievable> multiplier_retriever;
            std::shared_ptr<ComplexNumberDividable> divide_ops;
            std::shared_ptr<Conjugatable> conjugator;
            std::shared_ptr<InPlaceTransformable> dft;

            memory::linear::TempStorageGeneratable * temp_storage;
            
            const uint8_t DFT_THRESHOLD = 1; 

        public:

            FastInPlaceTransform(std::shared_ptr<ComplexNumberPlusable> plus_ops,
                                std::shared_ptr<ComplexNumberMultipliable> multiply_ops,
                                std::shared_ptr<FFTPrecalculatedMultiplierRetrievable> multiplier_retriever,
                                std::shared_ptr<ComplexNumberDividable> divide_ops,
                                std::shared_ptr<Conjugatable> conjugator,
                                std::shared_ptr<InPlaceTransformable> dft,

                                 memory::linear::TempStorageGeneratable * temp_storage){
                
                this->plus_ops = plus_ops;
                this->multiply_ops = multiply_ops;
                this->multiplier_retriever = multiplier_retriever;
                this->divide_ops = divide_ops;
                this->conjugator = conjugator;
                this->dft = dft;
                this->temp_storage = temp_storage;

            }

            void transform(ComplexNumber * data, size_t length){

                assert(length != 0);
                assert((length & (length - 1)) == 0);

                this->temp_storage->enter_scope();

                this->multiplier_retriever->prep(length, this->temp_storage->get(this->multiplier_retriever->get_instantiation_cost(length)));
                ComplexNumber * temp_space = (ComplexNumber *) this->temp_storage->get(sizeof(ComplexNumber) * length);
                this->transform(data, length, temp_space);

                this->temp_storage->exit_scope();

            }

            void inverse_transform(ComplexNumber * data, size_t length){

                auto cjg_lambda = [ops = this->conjugator](ComplexNumber e){return ops->conjugate(e);};
                auto dv_lambda = [ops = this->divide_ops, length](ComplexNumber e){return ops->divide(e, length);}; 

                linear_transform(data, length, cjg_lambda);
                this->transform(data, length);
                linear_transform(data, length, cjg_lambda);
                linear_transform(data, length, dv_lambda);

            }

        private:

            void transform(ComplexNumber * data, size_t length, ComplexNumber * temp_space){

                if (length <= DFT_THRESHOLD){

                    this->dft->transform(data, length);

                    return;

                }

                size_t half_length = length >> 1; 
                size_t i = 0;

                for (i = 0; i < length; i += 2){

                    temp_space[i >> 1] = data[i]; 

                }

                for (i = 1; i < length; i += 2){

                    temp_space[half_length + (i >> 1)] = data[i];

                }

                this->transform(temp_space, half_length, data);
                this->transform(temp_space + half_length, half_length, data + half_length);

                for (i = 0; i < half_length; ++i){

                    data[i] = this->plus_ops->plus(temp_space[i], this->multiply_ops->multiply(temp_space[i + half_length], this->multiplier_retriever->get(i, length))); 

                }

                for (i = 0; i < half_length; ++i){

                    data[i + half_length] = this->plus_ops->plus(temp_space[i], this->multiply_ops->multiply(temp_space[i + half_length], this->multiplier_retriever->get(i + half_length, length))); 

                }      

            }


    };
    
    class CacheEfficientFastInPlaceTransform: public InPlaceTransformable{

        private:

            typedef void (*lambda)(ComplexNumber *, ComplexNumber *);

            StandardComplexNumberPluser plus_ops;
            StandardComplexNumberMultiplier multiply_ops;
            StandardArchivedFFTPrecalculatedMultiplierRetriever mul_retriever;
            
            std::shared_ptr<ComplexNumberDividable> divide_ops;
            std::shared_ptr<Conjugatable> conjugator;
            memory::linear::TempStorageGeneratable * temp_storage;
            
        public:

            CacheEfficientFastInPlaceTransform(std::shared_ptr<ComplexNumberDividable> divide_ops,
                                               std::shared_ptr<Conjugatable> conjugator,

                                               memory::linear::TempStorageGeneratable * temp_storage){
                
                this->divide_ops = divide_ops;
                this->conjugator = conjugator;

                this->temp_storage = temp_storage;

            }

            void transform(ComplexNumber * data, size_t length){

                assert(length != 0);
                assert((length & (length - 1)) == 0);

                this->temp_storage->enter_scope();
                
                this->mul_retriever.prep(length, this->temp_storage->get(this->mul_retriever.get_instantiation_cost(length)));
                ComplexNumber * temp_space = (ComplexNumber *) this->temp_storage->get(length * sizeof(ComplexNumber) * 2);
                
                for (size_t i = 0; i < length * 2; i += 2){

                    temp_space[i] = data[i >> 1];

                }

                this->transform(temp_space, length, temp_space + 1);

                for (size_t i = 0; i < length * 2; i += 2){

                    data[i >> 1] = temp_space[i];

                }

                this->temp_storage->exit_scope();

            }

            void inverse_transform(ComplexNumber * data, size_t length){

                auto cjg_lambda = [ops = this->conjugator](ComplexNumber e){return ops->conjugate(e);};
                auto dv_lambda = [ops = this->divide_ops, length](ComplexNumber e){return ops->divide(e, length);}; 

                linear_transform(data, length, cjg_lambda);
                this->transform(data, length);
                linear_transform(data, length, cjg_lambda);
                linear_transform(data, length, dv_lambda);

            }

        private:

            void transform(ComplexNumber * data, size_t length, ComplexNumber * temp_space){

                if (length <= 1){

                    return;

                }

                size_t half_length = length >> 1; 
                size_t i = 0;

                for (i = 0; i < length; i += 2){

                    temp_space[i] = data[i << 1]; 
                    temp_space[i + length] = data[(i + 1) << 1]; 

                }

                this->transform(temp_space, half_length, data);
                this->transform(temp_space + length, half_length, data + length);

                for (i = 0; i < length; i += 2){  
                    
                    // data[i] = this->plus_ops.plus(temp_space[i], this->mul_retriever.get_rvalue(i >> 1, length)); 
                    // data[i + length] = this->plus_ops.plus(temp_space[i], this->mul_retriever.get_rvalue((i >> 1) + half_length, length)); 

                    data[i] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i + length], this->mul_retriever.get_rvalue(i >> 1, length))); 
                    data[i + length] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i + length], this->mul_retriever.get_rvalue((i >> 1) + half_length, length))); 

                }

            }

    };

    class CacheEfficientFastInPlaceTransformv2: public InPlaceTransformable{

        private:

            typedef void (*lambda)(ComplexNumber *, ComplexNumber *);

            StandardComplexNumberPluser plus_ops;
            StandardComplexNumberMultiplier multiply_ops;
            StandardArchivedFFTPrecalculatedMultiplierRetriever mul_retriever;
            
            std::shared_ptr<ComplexNumberDividable> divide_ops;
            std::shared_ptr<Conjugatable> conjugator;
            memory::linear::TempStorageGeneratable * temp_storage;
            
        public:

            CacheEfficientFastInPlaceTransformv2(std::shared_ptr<ComplexNumberDividable> divide_ops,
                                                 std::shared_ptr<Conjugatable> conjugator,

                                                 memory::linear::TempStorageGeneratable * temp_storage){
                
                this->divide_ops = divide_ops;
                this->conjugator = conjugator;

                this->temp_storage = temp_storage;

            }

            void transform(ComplexNumber * data, size_t length){

                assert(length != 0);
                assert((length & (length - 1)) == 0);

                this->temp_storage->enter_scope();
                
                this->mul_retriever.prep(length, this->temp_storage->get(this->mul_retriever.get_instantiation_cost(length)));
                ComplexNumber * temp_space = (ComplexNumber *) this->temp_storage->get(length * sizeof(ComplexNumber));
                this->transform(data, length, temp_space);

                this->temp_storage->exit_scope();

            }

            void inverse_transform(ComplexNumber * data, size_t length){

                auto cjg_lambda = [ops = this->conjugator](ComplexNumber e){return ops->conjugate(e);};
                auto dv_lambda = [ops = this->divide_ops, length](ComplexNumber e){return ops->divide(e, length);}; 

                linear_transform(data, length, cjg_lambda);
                this->transform(data, length);
                linear_transform(data, length, cjg_lambda);
                linear_transform(data, length, dv_lambda);

            }

        private:

            void transform(ComplexNumber * data, size_t length, ComplexNumber * temp_space){

                if (length <= 1){

                    return;

                }

                size_t half_length = length >> 1; 
                size_t i = 0;

                for (i = 0; i < half_length; ++i){
                    
                    temp_space[i] = data[i << 1]; 
                    temp_space[i + half_length] = data[(i << 1) | 1]; 

                }

                this->transform(temp_space, half_length, data);
                this->transform(temp_space + half_length, half_length, data);

                for (i = 0; i < half_length; ++i){  
                    
                    size_t i_half_length = i + half_length; 
                    
                    data[i] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i_half_length], this->mul_retriever.get_rvalue(i, length))); 
                    data[i_half_length] = this->plus_ops.plus(temp_space[i], this->multiply_ops.multiply(temp_space[i_half_length], this->mul_retriever.get_rvalue(i_half_length, length))); 

                }

            }

    };
   
    class StandardFastInPlaceTransformGenerator: public virtual InPlaceTransformGeneratable{

        public:

            std::shared_ptr<InPlaceTransformable> get(memory::linear::TempStorageGeneratable * allocator){

                auto plus_ops = std::shared_ptr<StandardComplexNumberPluser>(new StandardComplexNumberPluser());
                auto multiply_ops = std::shared_ptr<StandardComplexNumberMultiplier>(new StandardComplexNumberMultiplier());
                auto multiplier = std::shared_ptr<StandardFFTMultiplierRetriever>(new StandardFFTMultiplierRetriever());
                auto cached_multiplier = std::shared_ptr<TunedFFTPrecalculatedMultiplierRetriever>(new TunedFFTPrecalculatedMultiplierRetriever());

                auto divide_ops = std::shared_ptr<StandardComplexNumberDivider>(new StandardComplexNumberDivider());
                auto conjugator = std::shared_ptr<StandardConjugator>(new StandardConjugator());

                auto dft = std::shared_ptr<DFT>(new DFT(plus_ops, multiply_ops, multiplier, divide_ops, conjugator, allocator));
                //auto fft = std::shared_ptr<FastInPlaceTransform>(new FastInPlaceTransform(plus_ops, multiply_ops, cached_multiplier, divide_ops, conjugator, dft, allocator));
                
                auto fft = std::shared_ptr<CacheEfficientFastInPlaceTransformv2>(new CacheEfficientFastInPlaceTransformv2(divide_ops, conjugator, allocator));

                return fft;

            }

    };
    
    class LinearConvolutor: public virtual LinearConvolutable{

        private:    

            std::shared_ptr<InPlaceTransformable> ft;
            std::shared_ptr<ComplexNumberMultipliable> multiply_ops;
            std::shared_ptr<LinearProjectable> projector;

            memory::linear::TempStorageGeneratable * allocator;

        public: 

            LinearConvolutor(std::shared_ptr<InPlaceTransformable> ft,
                             std::shared_ptr<ComplexNumberMultipliable> multiply_ops,
                             std::shared_ptr<LinearProjectable> projector,

                             memory::linear::TempStorageGeneratable * allocator){

                this->ft = ft;
                this->multiply_ops = multiply_ops;
                this->projector = projector;
                this->allocator = allocator;

            }

            void convolute(double * lhs, double * rhs, size_t n, double * rs){
                
                assert(n != 0);
                assert((n & (n - 1)) == 0);

                this->allocator->enter_scope();

                ComplexNumber * projected_lhs = (ComplexNumber *) this->allocator->get(sizeof(ComplexNumber) * n);
                ComplexNumber * projected_rhs = (ComplexNumber *) this->allocator->get(sizeof(ComplexNumber) * n);

                this->projector->to_im_plane(lhs, projected_lhs, n);
                this->projector->to_im_plane(rhs, projected_rhs, n);
                
                this->ft->transform(projected_lhs, n);
                this->ft->transform(projected_rhs, n);
                
                for (size_t i = 0; i < n; ++i){

                    projected_lhs[i] = this->multiply_ops->multiply(projected_lhs[i], projected_rhs[i]); 

                }

                this->ft->inverse_transform(projected_lhs, n);
                this->projector->to_real_plane(projected_lhs, rs, n);

                this->allocator->exit_scope();

            }

    };

    class StandardLinearConvolutorGenerator{

        public:

            std::shared_ptr<LinearConvolutable> get(memory::linear::TempStorageGeneratable * allocator){

                auto fft = StandardFastInPlaceTransformGenerator().get(allocator); 
                auto multiply_ops = std::shared_ptr<StandardComplexNumberMultiplier>(new StandardComplexNumberMultiplier);
                auto projector = std::shared_ptr<LinearProjector>(new LinearProjector);

                return std::shared_ptr<LinearConvolutor>(new LinearConvolutor(fft, multiply_ops, projector, allocator)); 

            }

    };

}

namespace polynomial{

    class LinearSizeTDoubleCastable{

        public:

            virtual void to_double(size_t *, double *, size_t) = 0;
            virtual void to_size_t(double *, size_t *, size_t) = 0;

    };

    class FFTLengthComputable{

        public:

            virtual size_t get(size_t, size_t) = 0;

    };

    class UINTConvolutable{

        public:

            virtual size_t get_computation_space_cost(size_t, size_t) = 0; 
            virtual void convolute(size_t *, size_t,  size_t *, size_t, void *, size_t&) = 0;

    };

    class UINTConvolutorGeneratable{

        public:

            virtual std::shared_ptr<UINTConvolutable> get() = 0;

    };

};

namespace polynomial{

    class LinearSizeTDoubleCaster: public virtual LinearSizeTDoubleCastable{

        public:

            void to_double(size_t * data, double * projected, size_t n){
                
                const uint8_t double_prec = 52; 
                const size_t max_prec = (1ULL << double_prec) - 1; 

                for (size_t i = 0; i < n; ++i){

                    assert(data[i] <= max_prec);
                    projected[i] = data[i];

                }

            }

            void to_size_t(double * data, size_t * projected, size_t n){

                for (size_t i = 0; i < n; ++i){
                    
                    if (data[i] >= 0){
                        
                        projected[i] = std::round(data[i]);

                    } else{

                        projected[i] = 0;
                        
                    }

                }

            }

    }; 

    class FFTLengthComputer: public virtual FFTLengthComputable{

        public:

            size_t get(size_t lhs_sz, size_t rhs_sz){
                
                size_t virtual_length = std::max(lhs_sz, rhs_sz) << 1;

                if ((virtual_length & (virtual_length - 1)) == 0){

                    return virtual_length;

                }

                return 1 << (algorithm::log2::log2_64(virtual_length) + 1);

            }

    };

    class UINTConvolutor: public virtual UINTConvolutable{

        private:

            std::shared_ptr<fft::LinearConvolutable> convolutor;
            std::shared_ptr<LinearSizeTDoubleCastable> caster; 
            std::shared_ptr<FFTLengthComputable> fft_length_computer;

        public:

            UINTConvolutor(std::shared_ptr<fft::LinearConvolutable> convolutor,
                           std::shared_ptr<LinearSizeTDoubleCastable> caster,
                           std::shared_ptr<FFTLengthComputable> fft_length_computer){

                this->convolutor = convolutor;
                this->caster = caster;
                this->fft_length_computer = fft_length_computer;

            }

            size_t get_computation_space_cost(size_t lhs_sz, size_t rhs_sz){

                return this->get_rs_length(lhs_sz, rhs_sz) * sizeof(size_t)
                     + this->fft_length_computer->get(lhs_sz, rhs_sz) * sizeof(double) * 3;

            }

            void convolute(size_t * lhs, size_t lhs_sz, 
                           size_t * rhs, size_t rhs_sz, 
                           void * rs, size_t& rs_length){
                
                if (lhs_sz < rhs_sz){

                    this->convolute(rhs, rhs_sz, lhs, lhs_sz, rs, rs_length);

                    return;

                }

                size_t virtual_length = this->fft_length_computer->get(lhs_sz, rhs_sz);
                size_t padding_length = lhs_sz - rhs_sz;

                rs_length = this->get_rs_length(lhs_sz, rhs_sz);

                std::memset(rs, 0, this->get_computation_space_cost(lhs_sz, rhs_sz));

                double * casted_lhs = (double *) ((size_t *) rs + rs_length);
                double * casted_rhs = casted_lhs + virtual_length;
                double * casted_rs = casted_rhs + virtual_length;

                this->caster->to_double(lhs, casted_lhs, lhs_sz);
                this->caster->to_double(rhs, casted_rhs + padding_length, rhs_sz);
                
                this->convolutor->convolute(casted_lhs, casted_rhs, virtual_length, casted_rs);

                this->caster->to_size_t(casted_rs + padding_length, (size_t *) rs, rs_length);

            }
        
        private:

            size_t get_rs_length(size_t lhs_sz, size_t rhs_sz){

                return this->fft_length_computer->get(lhs_sz, rhs_sz) - std::max(lhs_sz, rhs_sz) + std::min(lhs_sz, rhs_sz);

            }

    };  

    class StandardUINTConvolutorGenerator: public virtual UINTConvolutorGeneratable{

        private:

            memory::linear::TempStorageGeneratable * allocator;
            
        public:

            StandardUINTConvolutorGenerator& set_temp_storage(memory::linear::TempStorageGeneratable * allocator){
                
                this->allocator = allocator;

                return *this;

            }

            std::shared_ptr<FFTLengthComputable> get_fft_length_computer(){

                return std::make_shared<FFTLengthComputer>(); 

            }

            std::shared_ptr<UINTConvolutable> get(){

                assert(this->allocator != nullptr);

                auto linear_convolutor = fft::StandardLinearConvolutorGenerator().get(this->allocator);
                auto caster = std::make_shared<LinearSizeTDoubleCaster>(); 
                auto fft_length_computer = std::make_shared<FFTLengthComputer>(); 

                return std::shared_ptr<UINTConvolutable>(new UINTConvolutor(linear_convolutor, caster, fft_length_computer)); 

            }

    };
}

namespace fft_multiplication{
    
    class BoolVectorViewSizeTCastable{

        public:

            virtual size_t get_instantiation_cost() = 0;
            virtual datastructure::UnalignedViewableBoolVector * get(size_t, void *) = 0;

    };

    class PaddedBoolVectorLinearSizeTCastable{

        public:
            
            virtual size_t get_instantiation_cost() = 0;
            virtual datastructure::UnalignedOperatableBoolVector * from_ptr(size_t *, size_t, size_t, void *) = 0;

    };

    class SplitLengthOptimizable{

        public:

            virtual size_t get(size_t, size_t) = 0; 

    };

    class BoolVectorDiscretizable{

        public:

            virtual size_t get_instantiation_cost(size_t, size_t) = 0;
            virtual void discretize(datastructure::UnalignedViewableBoolVector&, size_t,  void *, size_t&) = 0; 


    };

    class BoolVectorFusable{

        public:

            virtual size_t get_computation_space_cost() = 0;
            virtual void fuse(size_t *, size_t, size_t, datastructure::UnalignedOperatableBoolVector&, void *) = 0;

    };
    
    class BoolVectorMultipliable{

        public:

            virtual void multiply(datastructure::UnalignedViewableBoolVector&, datastructure::UnalignedViewableBoolVector&, 
                                  datastructure::UnalignedOperatableBoolVector&) = 0; 


    };

    class BoolVectorMultiplicationOperatorGeneratable{

        public:

            virtual std::shared_ptr<BoolVectorMultipliable> get() = 0;

    };

}

namespace fft_multiplication{
    
    namespace first_generation{
        
        class PaddedUnalignedBoolVectorOperator: public virtual datastructure::UnalignedOperatableBoolVector{

            private:

                datastructure::UnalignedOperatableBoolVector * data;
                size_t virtual_bit_width;
                size_t actual_bit_width;

            public:

                PaddedUnalignedBoolVectorOperator(datastructure::UnalignedOperatableBoolVector * data, 
                                                size_t virtual_bit_width, size_t actual_bit_width){
                    
                    this->data = data;
                    this->virtual_bit_width = virtual_bit_width;
                    this->actual_bit_width = actual_bit_width;

                }


                uint64_t length(){

                    return this->data->length() / actual_bit_width * virtual_bit_width;

                }

                uint64_t byte_length(){

                    size_t rs = this->length() / 8;

                    if (rs % 8 == 0){

                        return rs;

                    }

                    return rs + 1;

                }

                bool get(uint64_t idx){

                    return this->data->get(this->get_actual_idx(idx));

                }

                void set_val(uint64_t idx, bool val){

                    this->data->set_val(this->get_actual_idx(idx), val);

                }

            private:

                size_t get_actual_idx(size_t virtual_idx){

                    return this->get_slot(virtual_idx) * actual_bit_width + this->get_offset(virtual_idx);

                }

                size_t get_slot(size_t virtual_idx){

                    return virtual_idx / virtual_bit_width;

                }

                size_t get_offset(size_t virtual_idx){

                    return virtual_idx % virtual_bit_width; 

                }

        };

        class BoolVectorViewSizeTCaster: public virtual BoolVectorViewSizeTCastable{

            private:

                std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable> vec_generator;
                std::shared_ptr<memory::sizet_linear::caster::plm_boolvector::BoolVectorViewCastable> caster; 

            public:

                BoolVectorViewSizeTCaster(std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable> vec_generator,
                                        std::shared_ptr<memory::sizet_linear::caster::plm_boolvector::BoolVectorViewCastable> caster){
                    
                    this->vec_generator = vec_generator;
                    this->caster = caster;

                }

                size_t get_instantiation_cost(){

                    return this->vec_generator->get_instantiation_cost() + this->caster->get_instantiation_cost() + sizeof(size_t);

                }

                datastructure::UnalignedViewableBoolVector * get(size_t data, void * plm_space){

                    void * vec_space = (char *) plm_space + sizeof(size_t);
                    void * cast_space = (char *) vec_space  + vec_generator->get_instantiation_cost();

                    std::memcpy(plm_space, &data, sizeof(size_t));

                    auto vec = this->vec_generator->from_ptr(plm_space, 1, vec_space);
                    
                    return this->caster->cast(*vec, cast_space);

                }

        };

        class TrimmedBoolVectorViewSizeTCaster: public BoolVectorViewSizeTCaster{

            public:

                TrimmedBoolVectorViewSizeTCaster(std::shared_ptr<memory::sizet_linear::VectorReadableGeneratable> vec_generator,
                                                 std::shared_ptr<memory::sizet_linear::caster::plm_boolvector::BoolVectorViewCastable> caster): BoolVectorViewSizeTCaster(vec_generator, caster){}

                size_t get_instantiation_cost(){

                    return BoolVectorViewSizeTCaster::get_instantiation_cost() + sizeof(datastructure::UnalignedOffsetBoolVectorView);

                }

                datastructure::UnalignedViewableBoolVector * get(size_t data, void * plm_space){

                    void * casted_space = (char *) plm_space + sizeof(datastructure::UnalignedOffsetBoolVectorView);
                    auto rs = BoolVectorViewSizeTCaster::get(data, casted_space); 
                    auto bit_length = algorithm::log2::log2_64(data) + 1;

                    return new (plm_space) datastructure::UnalignedOffsetBoolVectorView(rs, 0, bit_length);

                }

        };

        class StandardBoolVectorViewSizeTCasterGenerator{

            public:

                std::shared_ptr<BoolVectorViewSizeTCastable> get(){

                    auto vec_generator = std::make_shared<memory::sizet_linear::StandardVectorReadableGenerator>();
                    auto caster = std::make_shared<memory::sizet_linear::caster::plm_boolvector::StandardBoolVectorViewCaster>();

                    return std::make_shared<BoolVectorViewSizeTCaster>(vec_generator, caster);

                }

        };

        class PaddedBoolVectorSizeTLinearCaster: public virtual PaddedBoolVectorLinearSizeTCastable{

            private:

                std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable> generator;
                std::shared_ptr<memory::sizet_linear::caster::plm_boolvector::BoolVectorOpCastable> caster;

                const uint8_t ACTUAL_BIT_WIDTH = sizeof(size_t) * 8;

            public:

                PaddedBoolVectorSizeTLinearCaster(std::shared_ptr<memory::sizet_linear::DanglingOperatableVectorGeneratable> generator,
                                                std::shared_ptr<memory::sizet_linear::caster::plm_boolvector::BoolVectorOpCastable> caster){
                    
                    this->generator = generator;
                    this->caster = caster;

                }

                size_t get_instantiation_cost(){

                    return this->generator->get_instantiation_cost() + this->caster->get_instantiation_cost() 
                        + sizeof(PaddedUnalignedBoolVectorOperator);

                }

                datastructure::UnalignedOperatableBoolVector * from_ptr(size_t * src, size_t src_n , size_t virtual_bit_width, void * plm_space){
                    
                    void * vec_space = plm_space;
                    void * caster_space = (char *) vec_space + this->generator->get_instantiation_cost();
                    void * pbv_space = (char *) caster_space + this->caster->get_instantiation_cost();

                    auto vec = this->generator->from_ptr(src, src_n, vec_space);
                    auto op_bvec = this->caster->cast(*vec, caster_space);

                    return new (pbv_space) PaddedUnalignedBoolVectorOperator(op_bvec, virtual_bit_width, ACTUAL_BIT_WIDTH);

                }

        };

        class StandardBoolVectorSizeTLinearCasterGenerator{

            public:

                std::shared_ptr<PaddedBoolVectorLinearSizeTCastable> get(){

                    auto vec_generator = std::make_shared<memory::sizet_linear::StandardDanglingOperatableVectorGenerator>();
                    auto caster = std::make_shared<memory::sizet_linear::caster::plm_boolvector::StandardBoolVectorOpCaster>();

                    return std::make_shared<PaddedBoolVectorSizeTLinearCaster>(vec_generator, caster);

                }

        };

        class SplitLengthOptimizer: public virtual SplitLengthOptimizable{

            private:

                std::shared_ptr<polynomial::FFTLengthComputable> fft_length_getter; 

            public:

                SplitLengthOptimizer(std::shared_ptr<polynomial::FFTLengthComputable> fft_length_getter){

                    this->fft_length_getter = fft_length_getter;

                }

                size_t get(size_t lhs_sz, size_t rhs_sz){
                    
                    const double MAX_NUMERICAL_ERROR = .25;
                    const size_t MAX_VAL_PER_SLOT = 1ULL << (sizeof(size_t) * 5);
                    const size_t DEFAULT_SPLIT_LENGTH = 5; 

                    size_t bp = 1;
                    size_t ep = MAX_VAL_PER_SLOT;

                    while (bp + 1 != ep){

                        size_t mid = (bp + ep) >> 1;
                        size_t n = this->get_fft_length(lhs_sz, rhs_sz, algorithm::log2::log2_64(mid)); 

                        if ((this->get_numerical_error(n, mid) < MAX_NUMERICAL_ERROR)){
                            
                            bp = mid;

                        } else{

                            ep = mid;

                        }

                    }

                    size_t cand = algorithm::log2::log2_64(bp) + 1;
                    
                    std::cout << "cand: " << cand << std::endl;
                    
                    return std::max(cand, DEFAULT_SPLIT_LENGTH);

                    //return 15;
                    
                }

            private:

                double get_numerical_error(size_t n, size_t b){
                    
                    return 1e-16 * 6 * b * b * n * n * algorithm::log2::log2_64(n); 
                    
                }

                size_t get_fft_length(size_t lhs_sz, size_t rhs_sz, size_t split_length){

                    size_t lhs_n = lhs_sz / split_length;
                    size_t rhs_n = rhs_sz / split_length;

                    if (lhs_n % split_length != 0) ++lhs_n;
                    if (rhs_n % split_length != 0) ++ rhs_n;

                    return this->fft_length_getter->get(lhs_n, rhs_n); 

                }


        };

        class StandardSplitLengthOptimizer: public virtual SplitLengthOptimizable{

            public:

                size_t get(size_t lhs_sz, size_t rhs_sz){

                    return 10;
                
                }

        };

        class BoolVectorDiscretizer: public virtual BoolVectorDiscretizable{
            
            private:

                std::shared_ptr<PaddedBoolVectorLinearSizeTCastable> caster;

            public:

                BoolVectorDiscretizer(std::shared_ptr<PaddedBoolVectorLinearSizeTCastable> caster){

                    this->caster = caster;

                }

                size_t get_instantiation_cost(size_t length, size_t bit_width){

                    return this->caster->get_instantiation_cost() + this->get_slot_size(length, bit_width) * sizeof(size_t);

                }

                void discretize(datastructure::UnalignedViewableBoolVector& data, size_t bit_width, void * op, size_t& op_n){
                    
                    op_n = this->get_slot_size(data.length(), bit_width);
                    void * caster_space = (char *) op + (op_n * sizeof(size_t));

                    std::memset(op, 0, op_n * sizeof(size_t));
                    
                    auto padded_bvec = this->caster->from_ptr((size_t *) op, op_n, bit_width, caster_space);
                    
                    for (size_t i = 0; i < data.length(); ++i){

                        padded_bvec->set_val(i, data.get(i));

                    }
                
                }

            
            private:

                size_t get_slot_size(size_t length, size_t bit_width){

                    size_t buf_size = length / bit_width;

                    if (length % bit_width != 0){
                        
                        ++buf_size;

                    } 

                    return buf_size;

                }


        };

        class StandardBoolVectorDiscretizerGenerator{
            
            public:

                std::shared_ptr<BoolVectorDiscretizable> get(){

                    auto caster = StandardBoolVectorSizeTLinearCasterGenerator().get();

                    return std::make_shared<BoolVectorDiscretizer>(caster); 

                }

        };

        class BoolVectorFuser: public virtual BoolVectorFusable{

            private:

                std::shared_ptr<BoolVectorViewSizeTCastable> caster;

            public:

                BoolVectorFuser(std::shared_ptr<BoolVectorViewSizeTCastable> caster){
                    
                    this->caster = caster;

                }

                size_t get_computation_space_cost(){

                    return this->caster->get_instantiation_cost();

                }

                void fuse(size_t * data, size_t n, size_t bit_width, datastructure::UnalignedOperatableBoolVector& op, void * plm_space){
                    
                    datastructure::UnalignedViewableBoolVector * bvec_view = nullptr;
                    size_t bit_offset = 0;

                    for (size_t i = 0; i < n; ++i){

                        bit_offset = i * bit_width;
                        bvec_view = this->caster->get(data[i], plm_space);

                        for (size_t j = 0; j < bvec_view->length(); ++j){

                            if (bvec_view->get(j)){
                                
                                this->add_bit(op, bit_offset + j);

                            }

                        }

                    }

                }

            private:

                void add_bit(datastructure::UnalignedOperatableBoolVector& op, size_t idx){

                    while ((idx < op.length()) && op.get(idx)){

                        op.set_val(idx, false);
                        ++idx;

                    }

                    assert(idx < op.length());

                    op.set_val(idx, true);

                }

        };

        class SizeTPlusOverflowDetector{

            private:

                const uint8_t SIZET_BIT_LENGTH = sizeof(size_t) * 8;

            public:

                bool plus(size_t lhs, size_t rhs, size_t rhs_l_shift){

                    size_t rhs_bit_length = rhs_l_shift + 1 + algorithm::log2::log2_64(rhs);
                    size_t lhs_bit_length = algorithm::log2::log2_64(lhs) + 1;

                    return std::max(rhs_bit_length, lhs_bit_length) + 1 > SIZET_BIT_LENGTH;

                }

        };

        class FastBoolVectorFuser: public virtual BoolVectorFusable{

            private:

                std::shared_ptr<BoolVectorViewSizeTCastable> caster;
                std::unique_ptr<SizeTPlusOverflowDetector> detector; 

            public:

                FastBoolVectorFuser(std::shared_ptr<BoolVectorViewSizeTCastable> caster){
                    
                    this->caster = caster;
                    this->detector = std::make_unique<SizeTPlusOverflowDetector>();

                }

                size_t get_computation_space_cost(){

                    return this->caster->get_instantiation_cost();

                }

                void fuse(size_t * data, size_t n, size_t bit_width, datastructure::UnalignedOperatableBoolVector& op, void * plm_space){
                    
                    size_t aggregated_data = 0;
                    
                    size_t l_shift = 0;
                    size_t relative_l_shift = 0; 

                    for (size_t i = 0; i < n; ++i){

                        if (!this->detector->plus(aggregated_data, data[i], relative_l_shift)){

                            aggregated_data += data[i] << relative_l_shift;
                            relative_l_shift += bit_width; 

                        } else{

                            datastructure::UnalignedViewableBoolVector * view = this->caster->get(aggregated_data, plm_space); 

                            for (size_t j = 0; j < view->length(); ++j){

                                if (view->get(j)){

                                    this->add_bit(op, l_shift + j);


                                }

                            }

                            aggregated_data = data[i];
                            l_shift = bit_width * i;
                            relative_l_shift = bit_width; 

                        }

                    }

                    if (aggregated_data){

                        datastructure::UnalignedViewableBoolVector * view = this->caster->get(aggregated_data, plm_space); 

                        for (size_t j = 0; j < view->length(); ++j){

                            if (view->get(j)){

                                this->add_bit(op, l_shift + j);


                            }

                        }
                    }

                }

            private:

                void add_bit(datastructure::UnalignedOperatableBoolVector& op, size_t idx){

                    while ((idx < op.length()) && op.get(idx)){

                        op.set_val(idx, false);
                        ++idx;

                    }

                    assert(idx < op.length());

                    op.set_val(idx, true);

                }

        };

        class StandardBoolVectorFuserGenerator{

            public:

                std::shared_ptr<BoolVectorFusable> get(){

                    auto caster = StandardBoolVectorViewSizeTCasterGenerator().get();

                    return std::make_shared<FastBoolVectorFuser>(caster);

                }

        };

        class BoolVectorMultiplicationOperator: public virtual BoolVectorMultipliable{

            private:

                std::shared_ptr<BoolVectorDiscretizable> discretizer;
                std::shared_ptr<BoolVectorFusable> fuser; 
                std::shared_ptr<polynomial::UINTConvolutable> uint_convolutor;
                std::shared_ptr<SplitLengthOptimizable> split_length_optimizer;

                memory::linear::TempStorageGeneratable * allocator;

            public:

                BoolVectorMultiplicationOperator(std::shared_ptr<BoolVectorDiscretizable> discretizer,
                                                std::shared_ptr<BoolVectorFusable> fuser,
                                                std::shared_ptr<polynomial::UINTConvolutable> uint_convolutor,
                                                std::shared_ptr<SplitLengthOptimizable> split_length_optimizer,

                                                memory::linear::TempStorageGeneratable * allocator){
                    
                    this->discretizer = discretizer;
                    this->fuser = fuser;
                    this->uint_convolutor = uint_convolutor;
                    this->split_length_optimizer = split_length_optimizer;

                    this->allocator = allocator;

                }

                void multiply(datastructure::UnalignedViewableBoolVector& lhs, datastructure::UnalignedViewableBoolVector& rhs, 
                            datastructure::UnalignedOperatableBoolVector& rs){
                    
                    assert((lhs.length() != 0) && (rhs.length() != 0));

                    this->allocator->enter_scope();

                    size_t virtual_bit_width = this->split_length_optimizer->get(lhs.length(), rhs.length());
                    
                    void * discreted_lhs = this->allocator->get(this->discretizer->get_instantiation_cost(lhs.length(), virtual_bit_width));
                    void * discreted_rhs = this->allocator->get(this->discretizer->get_instantiation_cost(rhs.length(), virtual_bit_width));

                    size_t discreted_lhs_n = 0;
                    size_t discreted_rhs_n = 0;

                    this->discretizer->discretize(lhs, virtual_bit_width, discreted_lhs, discreted_lhs_n);
                    this->discretizer->discretize(rhs, virtual_bit_width, discreted_rhs, discreted_rhs_n);

                    void * convoluted_rs = this->allocator->get(this->uint_convolutor->get_computation_space_cost(discreted_lhs_n, discreted_rhs_n));
                    size_t convoluted_n = 0; 

                    this->uint_convolutor->convolute((size_t *) discreted_lhs, discreted_lhs_n, (size_t *) discreted_rhs, discreted_rhs_n, convoluted_rs, convoluted_n);
                    this->fuser->fuse((size_t *) convoluted_rs, convoluted_n, virtual_bit_width, rs, this->allocator->get(this->fuser->get_computation_space_cost()));

                    this->allocator->exit_scope();

                }

        };

        class StandardBoolVectorMultiplicationOperatorGenerator: public virtual BoolVectorMultiplicationOperatorGeneratable{

            private:

                memory::linear::TempStorageGeneratable * allocator;

            public:

                StandardBoolVectorMultiplicationOperatorGenerator& set_temp_storage(memory::linear::TempStorageGeneratable * allocator){

                    this->allocator = allocator;

                    return *this;

                }

                std::shared_ptr<BoolVectorMultipliable> get(){

                    assert(this->allocator != nullptr);

                    auto fuser = StandardBoolVectorFuserGenerator().get();
                    auto discretizer = StandardBoolVectorDiscretizerGenerator().get();
                    auto uint_convolutor = polynomial::StandardUINTConvolutorGenerator().set_temp_storage(this->allocator).get();
                    //auto fft_length_computer = polynomial::StandardUINTConvolutorGenerator().get_fft_length_computer();
                    auto split_length_optimizer = std::make_shared<StandardSplitLengthOptimizer>();

                    return std::make_shared<BoolVectorMultiplicationOperator>(discretizer, fuser, uint_convolutor, split_length_optimizer, this->allocator);

                }

        };

    }

}

#endif