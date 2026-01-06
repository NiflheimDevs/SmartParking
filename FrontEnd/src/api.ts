import axios from "axios";

export const apiClient = axios.create({
  baseURL: "https://api.smartparking.niflheimdevs.ir/v1",
  headers: {
    "Content-Type": "application/json",
  },
});

apiClient.interceptors.request.use(
  (config) => {
    const token = localStorage.getItem("authToken");

    const publicRoutes = [
      "/login",
      "/signup",
    ];

    if (token && !publicRoutes.includes(config.url || "")) {
      config.headers.Authorization = `Bearer ${token}`;
    }

    return config;
  },
  (error) => {
    return Promise.reject(error);
  }
);

apiClient.interceptors.response.use(
  (response) => {
    return response;
  },
  async (error) => {
    if (error.response?.status === 401) {
      localStorage.removeItem("authToken");
      window.location.href = "/";
    }
    else if (error.response?.status === 403) {
      window.dispatchEvent(new Event("access-denied"));
    }
    return Promise.reject(error);
  }
);

export interface VehiclePayload {
  owner_name: string;
  owner_contact: string;
  plate: string;
  rfid_id: string;
  vehicle_type: string;
}

export interface AuthPayload {
  username: string;
  password: string;
}

// =====================================================
// GET ALL VEHICLES
// =====================================================
export const getAllVehicles = async () => {
  try {
    const response = await apiClient.get("/vehicles/");
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت لیست وسایل نقلیه!";
  }
};

// =====================================================
// GET SPECIFIC VEHICLE
// =====================================================
export const getVehicle = async (id: string | number) => {
  try {
    const response = await apiClient.get(`/vehicles/${id}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت اطلاعات وسیله نقلیه!";
  }
};

// =====================================================
// CREATE VEHICLE
// =====================================================
export const createVehicle = async (vehicleData: VehiclePayload) => {
  try {
    const response = await apiClient.post("/vehicles/", vehicleData);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در ثبت وسیله نقلیه جدید!";
  }
};

// =====================================================
// UPDATE VEHICLE
// =====================================================
export const updateVehicle = async (id: string | number, vehicleData: VehiclePayload) => {
  try {
    const response = await apiClient.put(`/vehicles/${id}`, vehicleData);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در به‌روزرسانی اطلاعات وسیله نقلیه!";
  }
};

// =====================================================
// DELETE VEHICLE
// =====================================================
export const deleteVehicle = async (id: string | number) => {
  try {
    const response = await apiClient.delete(`/vehicles/${id}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در حذف وسیله نقلیه!";
  }
};

export default apiClient;


export const getParkingSpaces = async () => {
  try {
    const response = await apiClient.get("/ps/");
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت لیست جایگاه‌های پارکینگ!";
  }
};

// Get all logs
export const getAllLogs = async () => {
  try {
    const response = await apiClient.get("/ee/");
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت لاگ‌ ورود و خروج!";
  }
};

// Get a specific log by ID
export const getLogById = async (id: number | string) => {
  try {
    const response = await apiClient.get(`/ee/${id}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت لاگ مورد نظر!";
  }
};

// Get logs for a specific vehicle
export const getLogsByVehicle = async (vehicleId: number | string) => {
  try {
    const response = await apiClient.get(`/ee/vehicle/${vehicleId}`);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در دریافت لاگ‌های این وسیله نقلیه!";
  }
};

export const login = async (data: AuthPayload) => {
  try {
    const response = await apiClient.post("/login",data);
    const accessToken = response.data.token;
    if (accessToken) {
      localStorage.setItem("authToken",accessToken)
    }
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در ورود"
  }
};

export const signup = async (data: AuthPayload) => {
  try {
    const response = await apiClient.post("/signup",data);
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در ثبت نام"
  }
};

export const controlgate = async (gate: string, state: boolean) => {
  try {
    const response = await apiClient.post("/gate/control", { gate, state });
    return response.data;
  } catch (error: any) {
    throw error.response?.data || "خطا در کنترل گیت!";
  }
}