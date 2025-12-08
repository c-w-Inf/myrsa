import argparse
import os
import sys
from pathlib import Path
from typing import Dict, Optional

# 检查并导入必要的库
try:
    from cryptography.hazmat.primitives import serialization
    from cryptography.hazmat.primitives.asymmetric import rsa
    from cryptography.hazmat.backends import default_backend
    from cryptography.hazmat.primitives.asymmetric.rsa import RSAPrivateNumbers, RSAPublicNumbers
except ImportError:
    print("错误: 需要安装 cryptography 库")
    print("请运行: pip install cryptography")
    sys.exit(1)


class RSAKeyGenerator:
    """RSA密钥生成器"""
    
    def __init__(self, params: Dict[str, int]):
        """
        初始化RSA密钥生成器
        
        Args:
            params: RSA参数字典，应包含以下键：
                - n: 模数
                - e: 公钥指数
                - d: 私钥指数
                - p: 第一个质数
                - q: 第二个质数
        """
        self.params = params
        self.private_key = None
        self.public_key = None
        
    def validate_params(self) -> bool:
        """验证参数是否完整"""
        required_keys = {'n', 'e', 'd', 'p', 'q'}
        missing_keys = required_keys - set(self.params.keys())
        
        if missing_keys:
            print(f"错误: 缺少必要的RSA参数: {missing_keys}")
            print("需要参数: n, e, d, p, q")
            return False
            
        # 检查参数是否为整数
        for key, value in self.params.items():
            if not isinstance(value, int):
                print(f"错误: 参数 {key} 必须是整数，当前类型: {type(value)}")
                return False
                
        return True
    
    def generate_keys(self):
        """从参数生成RSA密钥"""
        n = self.params['n']
        e = self.params['e']
        d = self.params['d']
        p = self.params['p']
        q = self.params['q']
        
        # 计算CRT参数
        dmp1 = d % (p - 1)  # d mod (p-1)
        dmq1 = d % (q - 1)  # d mod (q-1)
        iqmp = rsa.rsa_crt_iqmp(p, q)  # q^-1 mod p
        
        # 创建RSA私钥对象
        private_numbers = RSAPrivateNumbers(
            p=p,
            q=q,
            d=d,
            dmp1=dmp1,
            dmq1=dmq1,
            iqmp=iqmp,
            public_numbers=RSAPublicNumbers(e=e, n=n)
        )
        
        self.private_key = private_numbers.private_key(default_backend())
        self.public_key = self.private_key.public_key()
        
        print("✓ RSA密钥已成功生成")
        return True
    
    def save_private_key(self, filename: str = "id_rsa", passphrase: Optional[str] = None):
        """
        保存私钥到文件
        
        Args:
            filename: 输出文件名
            passphrase: 可选密码（用于加密私钥）
        """
        if not self.private_key:
            raise ValueError("尚未生成私钥")
        
        # 确定加密算法
        if passphrase:
            encryption_algorithm = serialization.BestAvailableEncryption(passphrase.encode())
            print(f"✓ 私钥将使用密码加密")
        else:
            encryption_algorithm = serialization.NoEncryption()
            print(f"⚠ 注意: 私钥未加密，建议设置密码")
        
        # 写入私钥文件
        with open(filename, 'wb') as f:
            f.write(self.private_key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.TraditionalOpenSSL,
                encryption_algorithm=encryption_algorithm
            ))
        
        # 设置正确的文件权限（仅限Unix/Linux）
        if os.name == 'posix':
            os.chmod(filename, 0o600)
        
        print(f"私钥已保存到: {filename}")
        return filename
    
    def save_public_key(self, filename: str = "id_rsa.pub", comment: str = ""):
        """
        保存公钥到文件
        
        Args:
            filename: 输出文件名
            comment: 公钥注释（通常为邮箱地址）
        """
        if not self.public_key:
            raise ValueError("尚未生成公钥")
        
        # 生成OpenSSH格式的公钥
        public_bytes = self.public_key.public_bytes(
            encoding=serialization.Encoding.OpenSSH,
            format=serialization.PublicFormat.OpenSSH
        )
        
        # 添加注释
        if comment:
            public_key_str = public_bytes.decode('utf-8').strip() + ' ' + comment + '\n'
        else:
            public_key_str = public_bytes.decode('utf-8') + '\n'
        
        # 写入公钥文件
        with open(filename, 'w') as f:
            f.write(public_key_str)
        
        print(f"公钥已保存到: {filename}")
        return filename
    
    def get_key_info(self):
        """获取密钥信息"""
        if not self.private_key:
            raise ValueError("尚未生成私钥")
        
        private_numbers = self.private_key.private_numbers()
        public_numbers = private_numbers.public_numbers
        
        info = {
            "key_size": public_numbers.n.bit_length(),
            "public_exponent": public_numbers.e,
            "modulus_hex": hex(public_numbers.n)[:50] + "..." if len(hex(public_numbers.n)) > 50 else hex(public_numbers.n),
            "key_type": "RSA",
            "fingerprint_md5": None,
            "fingerprint_sha256": None
        }
        
        return info


def parse_arguments():
    """解析命令行参数"""
    parser = argparse.ArgumentParser(
        description="从RSA参数生成SSH密钥文件",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
示例:
  1. 从JSON文件生成:
     python rsa_to_ssh.py -f params.json
  
  2. 使用命令行参数生成:
     python rsa_to_ssh.py -n 12345... -e 65537 -d 67890... -p 111... -q 222...
  
  3. 从JSON文件生成并加密私钥:
     python rsa_to_ssh.py -f params.json --passphrase "my_password"
  
  4. 指定输出目录和文件名:
     python rsa_to_ssh.py -f params.json -o /path/to/keys/ --private-name my_key --public-name my_key.pub
  
参数文件格式 (JSON):
  {
    "n": 12345678901234567890,
    "e": 65537,
    "d": 98765432109876543210,
    "p": 11111111111111111111,
    "q": 22222222222222222222
  }
  
注意: 参数可以是十进制整数或十六进制字符串（以0x开头）
        """
    )
    
    # 输入选项
    input_group = parser.add_argument_group('输入选项')
    input_group.add_argument('-f', '--file', type=str, help='包含RSA参数的JSON文件路径')
    input_group.add_argument('-n', '--modulus', type=str, help='模数 n')
    input_group.add_argument('-e', '--exponent', type=str, help='公钥指数 e')
    input_group.add_argument('-d', '--private-exponent', type=str, help='私钥指数 d')
    input_group.add_argument('-p', '--prime1', type=str, help='质数 p')
    input_group.add_argument('-q', '--prime2', type=str, help='质数 q')
    
    # 输出选项
    output_group = parser.add_argument_group('输出选项')
    output_group.add_argument('-o', '--output-dir', type=str, default='.', help='输出目录（默认：当前目录）')
    output_group.add_argument('--private-name', type=str, default='id_rsa', help='私钥文件名（默认：id_rsa）')
    output_group.add_argument('--public-name', type=str, default='id_rsa.pub', help='公钥文件名（默认：id_rsa.pub）')
    
    # 安全选项
    security_group = parser.add_argument_group('安全选项')
    security_group.add_argument('--passphrase', type=str, help='私钥加密密码')
    security_group.add_argument('--comment', type=str, default='', help='公钥注释（如邮箱地址）')
    
    # 其他选项
    parser.add_argument('--show-info', action='store_true', help='显示生成的密钥信息')
    parser.add_argument('--test', action='store_true', help='测试模式（不保存文件）')
    
    return parser.parse_args()


def parse_param_value(value: str):
    """解析参数值，支持十进制和十六进制"""
    if value.lower().startswith('0x'):
        return int(value, 16)
    else:
        return int(value)


def load_params_from_file(filepath: str) -> Dict[str, int]:
    """从JSON文件加载参数"""
    try:
        import json
        with open(filepath, 'r') as f:
            data = json.load(f)
        
        # 转换所有参数为整数
        params = {}
        for key in ['n', 'e', 'd', 'p', 'q']:
            if key not in data:
                raise ValueError(f"JSON文件中缺少参数: {key}")
            
            value = data[key]
            if isinstance(value, str):
                params[key] = parse_param_value(value)
            elif isinstance(value, int):
                params[key] = value
            else:
                raise ValueError(f"参数 {key} 必须是整数或字符串")
                
        return params
        
    except FileNotFoundError:
        # print(f"错误: 文件不存在: {filepath}")
        sys.exit(1)
    except json.JSONDecodeError:
        # print(f"错误: 文件不是有效的JSON格式: {filepath}")
        sys.exit(1)
    except ValueError as e:
        # print(f"错误: {e}")
        sys.exit(1)


def get_params_from_args(args) -> Dict[str, int]:
    """从命令行参数获取参数"""
    required_args = ['modulus', 'exponent', 'private_exponent', 'prime1', 'prime2']
    missing_args = [arg.replace('_', '-') for arg in required_args if not getattr(args, arg)]
    
    if missing_args:
        # print(f"错误: 缺少命令行参数: {', '.join(missing_args)}")
        # print("请使用 -h 查看帮助信息")
        sys.exit(1)
    
    try:
        return {
            'n': parse_param_value(args.modulus),
            'e': parse_param_value(args.exponent),
            'd': parse_param_value(args.private_exponent),
            'p': parse_param_value(args.prime1),
            'q': parse_param_value(args.prime2)
        }
    except ValueError as e:
        # print(f"错误: 参数解析失败: {e}")
        sys.exit(1)


def main():
    """主函数"""
    args = parse_arguments()
    
    # print("=" * 60)
    # print("RSA参数转SSH密钥文件生成器")
    # print("=" * 60)
    
    # 获取参数
    if args.file:
        # print(f"从文件加载参数: {args.file}")
        params = load_params_from_file(args.file)
    else:
        # print("从命令行参数加载")
        params = get_params_from_args(args)
    
    # 创建输出目录
    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    
    # 创建密钥生成器
    generator = RSAKeyGenerator(params)
    
    # 验证参数
    if not generator.validate_params():
        sys.exit(1)
    
    # print(f"✓ 参数验证通过")
    # print(f"  模数位数: {params['n'].bit_length()}")
    # print(f"  公钥指数: {params['e']}")
    
    # 生成密钥
    if not generator.generate_keys():
        # print("错误: 密钥生成失败")
        sys.exit(1)
    
    # 显示密钥信息
    if args.show_info:
        info = generator.get_key_info()
        # print("\n密钥信息:")
        # print(f"  密钥类型: {info['key_type']}")
        # print(f"  密钥长度: {info['key_size']} 位")
        # print(f"  模数 (前50位): {info['modulus_hex']}")
    
    # 测试模式
    if args.test:
        # print("\n测试模式: 不保存文件")
        # print("密钥生成成功，可以正常使用")
        return
    
    # 保存密钥文件
    private_path = output_dir / args.private_name
    public_path = output_dir / args.public_name
    
    print(f"\n保存文件到目录: {output_dir.absolute()}")
    
    try:
        generator.save_private_key(str(private_path), args.passphrase)
        generator.save_public_key(str(public_path), args.comment)
    except Exception as e:
        # print(f"错误: 保存文件失败: {e}")
        sys.exit(1)
    
    # 显示使用提示
    # print("\n" + "=" * 60)
    # print("生成完成!")
    # print("=" * 60)
    
    # if os.name == 'posix':
    #     print(f"\n使用以下命令测试密钥:")
    #     print(f"  # 测试私钥")
    #     print(f"  ssh-keygen -l -f {private_path}")
    #     print(f"  \n  # 测试公钥")
    #     print(f"  ssh-keygen -l -f {public_path}")
        
    #     if not args.passphrase:
    #         print(f"\n⚠ 安全提示: 私钥未加密，建议设置密码")
    #         print(f"  可以使用以下命令添加密码:")
    #         print(f"  ssh-keygen -p -f {private_path}")
    
    # print(f"\n私钥文件: {private_path.absolute()}")
    # print(f"公钥文件: {public_path.absolute()}")


if __name__ == "__main__":
    main()